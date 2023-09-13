#pragma once
#include "Hittable.h"
class Box :
    public Hittable
{
public : 

    Box();
    Box(Vec3 min, Vec3 max, int rotationX, int rotationY, int rotationZ, Material* m);


    virtual bool hit(const Ray& r, float tmin, float tmax, hit_record& rec) const;
    virtual void setAABB();

    virtual bool isPointInside(Vec3 point);


    Vec3 minPoint;
    Vec3 maxPoint;
    int rotDegX;
    int rotDegY;
    int rotDegZ;

    Matrix4 rotationMatrix;
    Matrix4 rotationMatrixInverse;
};

