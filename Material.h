#ifndef MATERIAL_H
#define MATERIAL_H



#include "Ray.h"
#include "Hittable.h"
#include "Random.h"

struct hit_record;

inline float schlick(float cosine, float ref_idx) {
    float r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}


inline bool refract(const Vec3& v, const Vec3& n, float ni_over_nt, Vec3& refracted) {
    Vec3 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
    if (discriminant > 0) {
        refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
        return true;
    }
    else
        return false;
}


inline Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - 2 * dot(v, n) * n;
}


inline Vec3 random_in_unit_sphere() {
    Vec3 p;
    do {
        p = 2.0 * Vec3(random_double(), random_double(), random_double()) - Vec3(1, 1, 1);
    } while (p.squared_length() >= 1.0);
    return p;
}

inline Vec3 randomDirectionWithBias(const Vec3& meanDirection, float biasFactor) {
    // Generate a random direction using spherical coordinates
    float phi = 2.0f * M_PI * static_cast<float>(rand()) / RAND_MAX;
    float theta = std::acos(1.0f - static_cast<float>(rand()) / RAND_MAX * (1.0f - std::cos(biasFactor)));

    // Calculate the random direction in spherical coordinates
    float x = std::sin(theta) * std::cos(phi);
    float y = std::sin(theta) * std::sin(phi);
    float z = std::cos(theta);

    Vec3 randomDir(x, y, z);

    // Calculate rotation matrix to rotate the random direction towards the mean direction
    //Vec3 rotationAxis = meanDirection.cross(randomDir).normalize();
    Vec3 rotationAxis = unit_vector(cross(meanDirection, randomDir));
    float rotationAngle = std::acos(dot(meanDirection,randomDir));

    // Rotate the random direction towards the mean direction
    randomDir = randomDir.rotate(rotationAxis, rotationAngle);

    return randomDir;
}

inline Vec3 randomDirectionInHemisphere(const Vec3& normal, const Vec3& biasDirection, float biasAngle) {
    Vec3 randomDir = random_in_unit_sphere();
    if (dot(randomDir,normal) < 0.0f) {
        randomDir = -randomDir;
    }

    Vec3 biasedDir = unit_vector((normal + biasDirection * biasAngle));
    Vec3 rotatedDir = randomDir * cos(biasAngle) + cross(biasedDir,randomDir) * sin(biasAngle);

    return rotatedDir;
}

class Material {
public:
    virtual bool scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const = 0;

    virtual Vec3 emitted(double u, double v, const Vec3& p) const {
        return Vec3(0.0, 0.0, 0.0);
    }
};


class lambertian : public Material {
public:
    lambertian(const Vec3& a) : albedo(a) {}
    virtual bool scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const {
        Vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = Ray(rec.p, target - rec.p);
        attenuation = albedo;
        return true;
    }

    Vec3 albedo;
};


class metal : public Material {
public:
    metal(const Vec3& a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }
    virtual bool scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const {
        Vec3 reflected = reflect(unit_vector(r_in.direction), rec.normal);
        scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction, rec.normal) > 0);
    }
    Vec3 albedo;
    float fuzz;
};

class light : public Material {
public:
    light(const Vec3& a) : color(a) {}

    Vec3 emitted(double u, double v, const Vec3& p) const {
        return color;
    }

    virtual bool scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const {
        return false;
    }

    Vec3  color;
};


class dielectric : public Material {
public:
    dielectric() { ref_idx = 1.0; }
    dielectric(float ri) : ref_idx(ri) {}
    virtual bool scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const {

        float prevRefInx=1.0;
        Vec3 outward_normal;
        Vec3 reflected = reflect(r_in.direction, rec.normal);
        float ni_over_nt;
        attenuation = Vec3(1.0, 1.0, 1.0);
        Vec3 refracted;
        float reflect_prob;
        float cosine;

        //check for smugglede information on previous medium
        if ((int)scattered.direction[0] == -1) {
            prevRefInx = scattered.direction[2];
        }

        if (dot(r_in.direction, rec.normal) > 0) {
            outward_normal = -rec.normal;
            ni_over_nt = ref_idx/prevRefInx;
            // cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
            cosine = dot(r_in.direction, rec.normal) / r_in.direction.length();
            cosine = sqrt(1 - ref_idx * ref_idx * (1 - cosine * cosine));
        }
        else {
            outward_normal = rec.normal;
            ni_over_nt = prevRefInx / ref_idx;
            cosine = -dot(r_in.direction, rec.normal) / r_in.direction.length();
        }
        if (refract(r_in.direction, outward_normal, ni_over_nt, refracted))
            reflect_prob = schlick(cosine, ref_idx);
        else
            reflect_prob = 1.0;
        if (random_double() < reflect_prob)
            scattered = Ray(rec.p, reflected);
        else
            scattered = Ray(rec.p, refracted);
        return true;
    }

    float ref_idx;
};

class opticalMedium : public dielectric {
public:
    opticalMedium(float ri, float scatterProb, const Vec3& a) : ref_idx(ri), scatteringProbability(scatterProb), albedo(a) {};

    opticalMedium(float ri, float scatterProb) : ref_idx(ri), scatteringProbability(scatterProb) { albedo = Vec3(0, 0, 0); }

    bool scatter_inside(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const {
        scattered = Ray(rec.p, random_in_unit_sphere());
        attenuation = albedo;

        return true;
    }

    virtual bool scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const {

        float prevRefInx = 1.0;
        Vec3 outward_normal;
        Vec3 reflected = reflect(r_in.direction, rec.normal);
        float ni_over_nt;
        attenuation = albedo;
        Vec3 refracted;
        float reflect_prob;
        float cosine;

        //check for smugglede information on previous medium
        if ((int)scattered.direction[0] == -1) {
            prevRefInx = scattered.direction[2];
        }

        if (dot(r_in.direction, rec.normal) > 0) {
            outward_normal = -rec.normal;
            ni_over_nt = ref_idx / prevRefInx;
            // cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
            cosine = dot(r_in.direction, rec.normal) / r_in.direction.length();
            cosine = sqrt(1 - ref_idx * ref_idx * (1 - cosine * cosine));
        }
        else {
            outward_normal = rec.normal;
            ni_over_nt = prevRefInx / ref_idx;
            cosine = -dot(r_in.direction, rec.normal) / r_in.direction.length();
        }
        if (refract(r_in.direction, outward_normal, ni_over_nt, refracted))
            reflect_prob = schlick(cosine, ref_idx);
        else
            reflect_prob = 1.0;
        if (random_double() < reflect_prob)
            scattered = Ray(rec.p, reflected);
        else
            scattered = Ray(rec.p, refracted);
        return true;
    }

    float ref_idx;
    float scatteringProbability;
    Vec3 albedo;
};

#endif
