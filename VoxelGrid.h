#ifndef VOXELGRID_H
#define VOXELGRID_H

#include "vec3.h"
#include "ray.h"
#include "Voxel.h"
#include "Hittable.h"

// VoxelGrid class declaration
class VoxelGrid {
public:
    int sizeX;
    int sizeY;
    int sizeZ;

    std::vector<Voxel*> voxels; //grid of voxels describing the scene
    Voxel mainVoxel;

    opticalMedium* outsideMaterial;


    VoxelGrid(int sizeX, int sizeY, int sizeZ);
    VoxelGrid(int sizeX, int sizeY, int sizeZ, Vec3 minPoint, Vec3 maxPoint);
    ~VoxelGrid();

    //bool isPointInsideVoxel(const Vec3& point, const Vec3& voxelMin, const Vec3& voxelMax) const;
    Vec3 getVoxelSize() const;
    Vec3 getVoxelArrayIndexes(const Vec3& point) const;
    Voxel* getVoxel(int x, int y, int z);
    void calculateVoxelBounds();

    // Function for fast voxel traversal algorithm
    Vec3 walkGrid(const Ray& ray, int depth);
    Vec3 walkGrid(const Ray& ray, int depth, Vec3 startingVoxelCoords); //like voxel grid but starting in specified voxel
    Vec3 walkGrid_new(const Ray& ray, int depth);
    Vec3 skybox(const Ray& ray);


    Vec3 CalculateStep(const Ray& ray)const;
    Vec3 CalculateDelta(const Ray& ray)const;


    /*
    void traverseRay(const Ray& ray, hit_record& rec);


    Vec3 traverseRay_new(const Ray& ray, hit_record& rec, int depth);

    Vec3 voxelMediumChange(const Ray& ray, Vec3 normal, float refractionRatio);

    //move to medium supporting class later
    Vec3 reflect(Vec3 rayIn, Vec3 normal);
    bool refract(Vec3 rayIn, Vec3 normal, float refractionRatio, Vec3& rayRefracted);
    */


    void addToVoxels(Hittable* primitive);
    void setPrimitivesToVoxels();
    void setAABB();
};

#endif // VOXELGRID_H


