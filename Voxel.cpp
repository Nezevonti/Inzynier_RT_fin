#include "Voxel.h"

Voxel::Voxel() : AABB(Vec3(), Vec3()){

    occupied = false;
}

Voxel::Voxel(opticalMedium* matPtr) {
    occupied = false;
    material = matPtr;
}

Voxel::Voxel(const Vec3& minPoint, const Vec3& maxPoint, opticalMedium* matPtr)
    : AABB(minPoint, maxPoint), material(matPtr) {
    occupied = false;

}

/*
void Voxel::setAABB(){

    for (Hittable* h : primitives) {

    }
}
*/

bool Voxel::primitivesHit(const Ray& ray, float tmin, float tmax, hit_record& rec) {
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = tmax;
    for (Hittable* prim : primitives) {
        //intersection with primitives bounding box, check it
        if (prim->boundingBox.intersect(ray, tmin, closest_so_far, temp_rec)) {
            if (prim->hit(ray, tmin, closest_so_far, temp_rec)) {
                //check if the hit occurs in this voxel
                //if (prim->boundingBox.isPointInside(temp_rec.p)) 
                if(true){
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
            }
        }
    }
    return hit_anything;
}