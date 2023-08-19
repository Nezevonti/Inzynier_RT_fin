#include "AABB.h"


AABB::AABB() {

}

AABB::AABB(const Vec3& minPoint, const Vec3& maxPoint)
    : minPoint(minPoint), maxPoint(maxPoint) {}


Vec3 AABB::calculateIntersectionPoint(const Ray& ray, float t) const {
    return Vec3(ray.origin.x() + t * ray.direction.x(),
        ray.origin.y() + t * ray.direction.y(),
        ray.origin.z() + t * ray.direction.z());
}

bool AABB::intersect(const Ray& ray, float t_min, float t_max, hit_record& rec) const{

    //Uses : Fast, Branchless Ray/Bounding Box Intersections
    //From : https://tavianator.com/2011/ray_box.html

    float t1[3];
    float t2[3];
    float tmin_direction[3];
    float tmax_direction[3];
    float tmin_overall = -INFINITY;
    float tmax_overall = INFINITY;

    for (int i = 0; i < 3; i++) {
        t1[i] = (minPoint[i] - ray.origin[i]) * ray.directionInverse[i];
        t2[i] = (maxPoint[i] - ray.origin[i]) * ray.directionInverse[i];

        tmin_direction[i] = (t1[i] < t2[i]) ? t1[i] : t2[i];
        tmax_direction[i] = (t1[i] > t2[i]) ? t1[i] : t2[i];

        tmin_overall = (tmin_overall > tmin_direction[i]) ? tmin_overall : tmin_direction[i];
        tmax_overall = (tmax_overall < tmax_direction[i]) ? tmax_overall : tmax_direction[i];
    }

    if (tmax_overall >= tmin_overall && tmax_overall >= 0) {
        rec.t = tmin_overall;
        rec.p = calculateIntersectionPoint(ray, tmin_overall);

        
        if (tmin_overall == tmin_direction[0])
            rec.normal= Vec3(-1, 0, 0);
        else if (tmin_overall == tmin_direction[1])
            rec.normal = Vec3(0, -1, 0);
        else if (tmin_overall == tmin_direction[2])
            rec.normal = Vec3(0, 0, -1);
        else if (tmin_overall == tmax_direction[0])
            rec.normal = Vec3(1, 0, 0);
        else if (tmin_overall == tmax_direction[1])
            rec.normal = Vec3(0, 1, 0);
        else if (tmin_overall == tmax_direction[2])
            rec.normal = Vec3(0, 0, 1);

        

        return true;
    }
    
    return false;
}

bool AABB::isPointInside(const Vec3& point){
    return point.x() >= this->minPoint.x() && point.x() <= this->maxPoint.x() &&
        point.y() >= this->minPoint.y() && point.y() <= this->maxPoint.y() &&
        point.z() >= this->minPoint.z() && point.z() <= this->maxPoint.z();
}

void AABB::padBB() {
    float delta = 0.01;

    
    float diffX = std::abs(maxPoint.x() - minPoint.x());
    float diffY = std::abs(maxPoint.y() - minPoint.y());
    float diffZ = std::abs(maxPoint.z() - minPoint.z());

    if (diffX <= delta) {
        this->maxPoint.e[0] += delta;
    }

    if (diffY <= delta) {
        this->maxPoint.e[1] += delta;
    }

    if (diffZ <= delta) {
        this->maxPoint.e[2] += delta;
    }

}

/*
bool AABB::isPointInside(const Vec3& point) const {
    return point.x() >= this.minPoint.x() && point.x() <= this.maxPoint.x() &&
        point.y() >= this.minPoint.y() && point.y() <= this.maxPoint.y() &&
        point.z() >= this.minPoint.z() && point.z() <= this.maxPoint.z();
}
*/