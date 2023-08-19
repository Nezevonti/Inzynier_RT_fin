#include "Hittable_List.h"

bool Hittable_List::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;
    for (int i = 0; i < list_size; i++) {
        if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}


void Hittable_List::setAABB() {
    /*
    Vec3 minPoint = Vec3(0,0,0);
    Vec3 maxPoint = Vec3(0, 0, 0);

    

    for (int i = 0; i < list_size; i++) {
        if (list[i]->boundingBox.minPoint.x() < minPoint.x()) {
            minPoint.e[0] = list[i]->boundingBox.minPoint.x();
        }
        if (list[i]->boundingBox.minPoint.y() < minPoint.y()) {
            minPoint.e[1] = list[i]->boundingBox.minPoint.y();
        }
        if (list[i]->boundingBox.minPoint.z() < minPoint.z()) {
            minPoint.e[2] = list[i]->boundingBox.minPoint.z();
        }
        if (list[i]->boundingBox.maxPoint.x() > maxPoint.x()) {
            minPoint.e[0] = list[i]->boundingBox.minPoint.x();
        }
        if (list[i]->boundingBox.maxPoint.y() > maxPoint.y()) {
            minPoint.e[1] = list[i]->boundingBox.minPoint.y();
        }
        if (list[i]->boundingBox.maxPoint.z() > maxPoint.z()) {
            minPoint.e[2] = list[i]->boundingBox.minPoint.z();
        }
    }

    boundingBox.maxPoint = maxPoint;
    boundingBox.minPoint = minPoint;
    */
}

