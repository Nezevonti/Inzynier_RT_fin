#include "VoxelGrid.h"

#include "Voxel.h"
#include "Material.h"

#include <typeinfo>


VoxelGrid::VoxelGrid(int sizeX, int sizeY, int sizeZ)
    : sizeX(sizeX), sizeY(sizeY), sizeZ(sizeZ) {

    int numVoxels = sizeX * sizeY * sizeZ;
    this->outsideMaterial = new opticalMedium(1.0, 0.0);

    for (int i = 0; i < numVoxels; i++) {
        voxels.push_back(new Voxel(new opticalMedium(1.0, 0.0)));
    }

    

    mainVoxel = Voxel(Vec3(0, 0, 0), Vec3(sizeX, sizeY, sizeZ), outsideMaterial);
    calculateVoxelBounds();
}

VoxelGrid::VoxelGrid(int sizeX, int sizeY, int sizeZ, Vec3 minPoint, Vec3 maxPoint)
    : sizeX(sizeX), sizeY(sizeY), sizeZ(sizeZ) {

    int numVoxels = sizeX * sizeY * sizeZ;

    this->outsideMaterial = new opticalMedium(1.0, 0.0);

    for (int i = 0; i < numVoxels; i++) {
        voxels.push_back(new Voxel(this->outsideMaterial));
    }



    mainVoxel = Voxel(minPoint, maxPoint,outsideMaterial);
    calculateVoxelBounds();

}

// VoxelGrid destructor implementation
VoxelGrid::~VoxelGrid() {
    for (auto p : voxels) {
        delete p;
    }
    voxels.clear();
}

//Moved to AABB class
/*
bool VoxelGrid::isPointInsideVoxel(const Vec3& point, const Vec3& voxelMin, const Vec3& voxelMax) const {
    return point.x() >= voxelMin.x() && point.x() <= voxelMax.x() &&
        point.y() >= voxelMin.y() && point.y() <= voxelMax.y() &&
        point.z() >= voxelMin.z() && point.z() <= voxelMax.z();
}
*/

Vec3 VoxelGrid::getVoxelSize() const {
    float voxelSizeX = (mainVoxel.maxPoint.x() - mainVoxel.minPoint.x()) / sizeX;
    float voxelSizeY = (mainVoxel.maxPoint.y() - mainVoxel.minPoint.y()) / sizeY;
    float voxelSizeZ = (mainVoxel.maxPoint.z() - mainVoxel.minPoint.z()) / sizeZ;

    return Vec3(voxelSizeX, voxelSizeY, voxelSizeZ);
}

Vec3 VoxelGrid::getVoxelArrayIndexes(const Vec3& point) const {

    //I had i there for a reason, im gonna comment it out but keep the code snippet
    // Check if the point is inside the main AABB
    //if (!isPointInsideVoxel(point, mainVoxel.minPoint, mainVoxel.maxPoint)) {
        //throw std::out_of_range("Vec3 index out of range");
    //}

    Vec3 voxelSize = getVoxelSize();

    int indexX = static_cast<int>((point.x() - mainVoxel.minPoint.x()) / voxelSize.x());
    int indexY = static_cast<int>((point.y() - mainVoxel.minPoint.y()) / voxelSize.y());
    int indexZ = static_cast<int>((point.z() - mainVoxel.minPoint.z()) / voxelSize.z());

    // Check if the point is on the edge or corner of the voxel grid
    if (indexX == sizeX || indexY == sizeY || indexZ == sizeZ) {
        // Check if the point does not belong to the edge/corner
        if (point.x() <= mainVoxel.maxPoint.x() || point.y() <= mainVoxel.maxPoint.y() || point.z() <= mainVoxel.maxPoint.z()) {
            // Adjust the indexes to the maximum valid value
            if (indexX == sizeX) indexX--;
            if (indexY == sizeY) indexY--;
            if (indexZ == sizeZ) indexZ--;
        }
        else {
            throw std::out_of_range("Vec3 index out of range");
        }
    }

    return Vec3(indexX, indexY, indexZ);
}

Voxel* VoxelGrid::getVoxel(int x, int y, int z) {
    int index = x + y * sizeX + z * sizeX * sizeY;
    return voxels.at(index);
}


void VoxelGrid::calculateVoxelBounds() {
    Vec3 voxelSize = getVoxelSize();

    for (int z = 0; z < sizeZ; ++z) {
        for (int y = 0; y < sizeY; ++y) {
            for (int x = 0; x < sizeX; ++x) {
                int index = z * sizeX * sizeY + y * sizeX + x;
                Voxel* voxel = voxels.at(index);


                voxel->minPoint = Vec3(x * voxelSize.x() + mainVoxel.minPoint.x(), y * voxelSize.y() + mainVoxel.minPoint.y(), z * voxelSize.z() + mainVoxel.minPoint.z());
                voxel->maxPoint = voxel->minPoint + voxelSize;
            }
        }
    }
}

void VoxelGrid::setAABB() {
    Vec3 minPointLocal = Vec3(0, 0, 0);
    Vec3 maxPointLocal = Vec3(0, 0, 0);


    for (Hittable* h : mainVoxel.primitives) {
        if (h->boundingBox.minPoint.x() < minPointLocal.x()) {
            minPointLocal.e[0] = h->boundingBox.minPoint.x();
        }
        if (h->boundingBox.minPoint.y() < minPointLocal.y()) {
            minPointLocal.e[1] = h->boundingBox.minPoint.y();
        }
        if (h->boundingBox.minPoint.z() < minPointLocal.z()) {
            minPointLocal.e[2] = h->boundingBox.minPoint.z();
        }
        if (h->boundingBox.maxPoint.x() > maxPointLocal.x()) {
            maxPointLocal.e[0] = h->boundingBox.maxPoint.x();
        }
        if (h->boundingBox.maxPoint.y() > maxPointLocal.y()) {
            maxPointLocal.e[1] = h->boundingBox.maxPoint.y();
        }
        if (h->boundingBox.maxPoint.z() > maxPointLocal.z()) {
            maxPointLocal.e[2] = h->boundingBox.maxPoint.z();
        }
    

        mainVoxel.maxPoint = maxPointLocal;
        mainVoxel.minPoint = minPointLocal;
    }
}

void VoxelGrid::setPrimitivesToVoxels() {

    for (Hittable* h : mainVoxel.primitives) {
        addToVoxels(h);

        //check if light
        Vec3 emitted = h->mat_ptr->emitted(0, 0, Vec3(0, 0, 0));
        if (emitted.e[0] != 0.0 && emitted.e[1] != 0.0 && emitted.e[2] != 0.0 ) {
            this->lights.push_back(h);
        }
    }

}


void VoxelGrid::addToVoxels(Hittable* primitive) {
    const AABB& bBox = primitive->boundingBox;
    Vec3 bBoxminPoint = bBox.minPoint;  //point
    Vec3 bBoxmaxPoint = bBox.maxPoint;  //point

    Vec3 voxelSize = getVoxelSize();
    Vec3 minVoxelIdx = getVoxelArrayIndexes(bBoxminPoint);  //index of voxel that point is in
    Vec3 maxVoxelIdx = getVoxelArrayIndexes(bBoxmaxPoint);  //index of voxel that point is in


    Vec3 tmp_min = mainVoxel.minPoint + Vec3(FLT_EPSILON, FLT_EPSILON, FLT_EPSILON); //point
    Vec3 test_min = getVoxelArrayIndexes(tmp_min); //index of voxel that point is in
    Vec3 tmp_max = mainVoxel.maxPoint - Vec3(2 * FLT_EPSILON, 2 * FLT_EPSILON, 2 * FLT_EPSILON); //point
    Vec3 test_max = getVoxelArrayIndexes(tmp_max); //index of voxel that point is in


    Voxel* min_voxel = getVoxel(test_min.x(), test_min.y(), test_min.z());
    Voxel* max_voxel = getVoxel(test_max.x(), test_max.y(), test_max.z());

    // Iterate over the voxels within the bounding box
    for (int i = minVoxelIdx.x(); i <= maxVoxelIdx.x(); ++i) {
        for (int j = minVoxelIdx.y(); j <= maxVoxelIdx.y(); ++j) {
            for (int k = minVoxelIdx.z(); k <= maxVoxelIdx.z(); ++k) {
                Vec3 voxelMinPoint = getVoxel(i, j, k)->minPoint;  //point
                Vec3 voxelMaxPoint = getVoxel(i, j, k)->maxPoint;  //point

                if (bBoxminPoint.x() < voxelMaxPoint.x() && bBoxmaxPoint.x() > voxelMinPoint.x() &&
                    bBoxminPoint.y() < voxelMaxPoint.y() && bBoxmaxPoint.y() > voxelMinPoint.y() &&
                    bBoxminPoint.z() < voxelMaxPoint.z() && bBoxmaxPoint.z() > voxelMinPoint.z()) {
                    getVoxel(i, j, k)->primitives.push_back(primitive);
                    getVoxel(i, j, k)->occupied = true;
                }
            }
        }
    }
}

Vec3 VoxelGrid::skybox(const Ray& ray) {

    //eturn Vec3(0.0001, 0.0001, 0.0001);
    return Vec3(0.0005, 0.0005, 0.0005);
    
    /*
    Vec3 unit_direction = unit_vector(ray.direction);
    float t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Vec3(0.25, 0.25, 0.25) + t * Vec3(0.5, 0.7, 1.0);
    */
    
}

Vec3 VoxelGrid::CalculateStep(const Ray& ray)const {
    int stepX = (ray.direction.x() >= 0) ? 1 : -1;
    int stepY = (ray.direction.y() >= 0) ? 1 : -1;
    int stepZ = (ray.direction.z() >= 0) ? 1 : -1;

    if (ray.direction.x() == 0.0) stepX = 0;
    if (ray.direction.y() == 0.0) stepY = 0;
    if (ray.direction.z() == 0.0) stepZ = 0;

    return Vec3(stepX, stepY, stepZ);
}


Vec3 VoxelGrid::GridWalk_Start(const Ray& ray) {

    hit_record rec;

    bool startsInside = mainVoxel.isPointInside(ray.origin);
    bool hitsScene = mainVoxel.intersect(ray, 0.001, FLT_MAX, rec);

    if (!hitsScene) {
        return skybox(ray);
    }

    if (startsInside) {
        //Calculate coords of the ray origin
        Vec3 indexes = getVoxelArrayIndexes(ray.origin);

        return GridWalk(ray, 0, (int)indexes.x(), (int)indexes.y(), (int)indexes.z());
    }

    //Ray starts outside the grid
    int nextX, nextY, nextZ;
    int currentX, currentY, currentZ;
    double tCurrent;
    Voxel* currentVoxel;
    Voxel* nextVoxel;
    opticalMedium* currentMaterial;
    opticalMedium* nextMaterial;

    //calculate the next voxel
    Vec3 nextVoxelIndexes = getVoxelArrayIndexes(rec.p);
    nextX = nextVoxelIndexes.x();
    nextY = nextVoxelIndexes.y();
    nextZ = nextVoxelIndexes.z();
    nextVoxel = getVoxel(nextX, nextY, nextZ);
    nextMaterial = nextVoxel->material;

    currentMaterial = outsideMaterial;
    
    if (nextMaterial->ref_idx == currentMaterial->ref_idx) {
        Ray newRay(rec.p, ray.direction);
        return GridWalk(newRay, 0, nextX, nextY, nextZ);
    }


    //Move to new func later on
    //bool ChangeMedium(Ray ray, hit_record rec, ray scattered, ray attenuation)
    //calculate normal to current voxel intersection
    bool b = nextVoxel->intersect(ray, 0.001, FLT_MAX, rec);
    Ray newRay;
    Vec3 attenuation;

    Vec3 tmpPoint = ray.at(rec.t); //debug only
    rec.mat_ptr = nextMaterial;

    //quick hack to pass current refractive index into the scatter function;
    newRay.direction[0] = -1;
    newRay.direction[2] = currentMaterial->ref_idx;

    if (b && rec.mat_ptr->scatter(ray, rec, attenuation, newRay)) { //Add passing current&previous 
        return GridWalk(newRay, 0, nextX, nextY, nextZ);
    }

    return skybox(ray);
}






Vec3 VoxelGrid::GridWalk(const Ray& ray, int depth, int VoxelX, int VoxelY, int VoxelZ) {

    hit_record rec;
    
    int stepX, stepY, stepZ;
    double tCurrent, tPrevious, tNext;
    int currentX = VoxelX, currentY = VoxelY, currentZ = VoxelZ;
    int nextX = currentX, nextY=currentY, nextZ=currentZ;
    Voxel* currentVoxel;
    Voxel* nextVoxel;
    opticalMedium* currentMaterial;
    opticalMedium* nextMaterial;
    Vec3 voxelSizes = getVoxelSize();
    Vec3 initialIntersectionPoint;

    Ray scattered;
    Vec3 attenuation;

    currentVoxel = getVoxel(currentX, currentY, currentZ);
    
    bool currhit = currentVoxel->intersect(ray, 0.001, FLT_MAX, rec);
    initialIntersectionPoint = rec.p;

    Vec3 gridSteps = CalculateStep(ray);
    stepX = (int)gridSteps.x();
    stepY = (int)gridSteps.y();
    stepZ = (int)gridSteps.z();

    //calculate tDeltas
    double invX = 1.0 / (double)ray.direction.x();
    double invY = 1.0 / (double)ray.direction.y();
    double invZ = 1.0 / (double)ray.direction.z();

    double tDeltaX = std::abs(invX) * voxelSizes.x();
    double tDeltaY = std::abs(invY) * voxelSizes.y();
    double tDeltaZ = std::abs(invZ) * voxelSizes.z();


    //calculate initial tMaxs
    double dx = (ray.direction.x() >= 0) ? std::abs(currentVoxel->maxPoint.x() - initialIntersectionPoint.x()) : std::abs(currentVoxel->minPoint.x() - initialIntersectionPoint.x());
    double dy = (ray.direction.y() >= 0) ? std::abs(currentVoxel->maxPoint.y() - initialIntersectionPoint.y()) : std::abs(currentVoxel->minPoint.y() - initialIntersectionPoint.y());
    double dz = (ray.direction.z() >= 0) ? std::abs(currentVoxel->maxPoint.z() - initialIntersectionPoint.z()) : std::abs(currentVoxel->minPoint.z() - initialIntersectionPoint.z());

    double tMaxX = std::abs(dx * invX);
    double tMaxY = std::abs(dy * invY);
    double tMaxZ = std::abs(dz * invZ);

    tCurrent = 0;
    tNext = tCurrent;
    tPrevious = tCurrent;

    //loop
    while (currentX >= 0 && currentX < sizeX &&
        currentY >= 0 && currentY < sizeY &&
        currentZ >= 0 && currentZ < sizeZ) {

        currentVoxel = getVoxel(currentX, currentY, currentZ);
        currentMaterial = currentVoxel->material;

        //
        //hit_record rec_tmp;
        //currentVoxel->intersect(ray, 0.000001, FLT_MAX, rec_tmp);


        //primitive in voxel?
        if (currentVoxel->occupied) {
            if (currentVoxel->primitivesHit(ray, 0.0001, FLT_MAX, rec)) {
                //a primitive was hit. Calculate the scattered ray
                //CHANGE MAX DEPTH TO VARIABLE!!!
                if (currentVoxel->isPointInside(rec.p)) {
                    
                    //quick hack to pass current refractive index into the scatter function;
                    scattered.direction[0] = -1;
                    scattered.direction[2] = currentVoxel->material->ref_idx;

                    if (depth < 25 && rec.mat_ptr->scatter(ray, rec, attenuation, scattered)) {
                        return rec.mat_ptr->emitted(rec.u,rec.v,rec.p) + (attenuation * GridWalk(scattered, depth + 1, currentX, currentY, currentZ));
                    }
                    else {
                        return rec.mat_ptr->emitted(rec.u, rec.v, rec.p);// +attenuation;
                    }
                }
                
            }
        }

        //Calculate next voxel
        if (tMaxX < tMaxY) {
            if (tMaxX < tMaxZ) {
                nextX += stepX;
                tNext = tMaxX;
                tMaxX += tDeltaX;

            }
            else {
                nextZ += stepZ;
                tNext = tMaxZ;
                tMaxZ += tDeltaZ;

            }
        }
        else {
            if (tMaxY < tMaxZ) {
                nextY += stepY;
                tNext = tMaxY;
                tMaxY += tDeltaY;

            }
            else {
                nextZ += stepZ;
                tNext = tMaxZ;
                tMaxZ += tDeltaZ;

            }
        }

        if (nextX > 0 && nextX < sizeX && nextY > 0 && nextY < sizeY && nextZ > 0 && nextZ < sizeZ) {
            nextVoxel = getVoxel(nextX, nextY, nextZ);
            nextMaterial = nextVoxel->material;


            if (currentMaterial->ref_idx != nextMaterial->ref_idx) {
                bool b = nextVoxel->intersect(ray, 0.001, FLT_MAX, rec);

                Vec3 tmpPoint = ray.at(rec.t); //debug only

                rec.mat_ptr = nextMaterial;

                scattered.direction[0] = -1;
                scattered.direction[2] = currentMaterial->ref_idx;

                if (b && rec.mat_ptr->scatter(ray, rec, attenuation, scattered)) { //Add passing current&previous 

                    Vec3 newSteps = CalculateStep(scattered);

                    if (newSteps.x() != stepX || newSteps.y() != stepY || newSteps.z() != stepZ) {
                        //reflected
                        return rec.mat_ptr->emitted(rec.u, rec.v, rec.p) + (attenuation * GridWalk(scattered, depth + 1, currentX, currentY, currentZ));
                    }
                    else {
                        //refracted
                        return rec.mat_ptr->emitted(rec.u, rec.v, rec.p) + (attenuation * GridWalk(scattered, depth + 1, nextX, nextY, nextZ));
                    }
 
                }
            }



            
            //Calculate traveled distance
            float tDiff = tNext - tCurrent;


            Vec3 pCurrent = ray.at(tCurrent);
            Vec3 pNext = ray.at(tNext);

            //OUT-SCATTERING

            float pTreshold = std::powf((1.0 - (currentVoxel->material->scatteringProbability)), tDiff);
            double random = random_double();
            if (random > pTreshold) {
                float tScatter = tNext - (tDiff * random_double()); //get a random t somewhere in the voxel
                hit_record tmprec;
                tmprec.p = ray.at(tScatter);
                currentVoxel->material->scatter_inside(ray, tmprec, attenuation, scattered);

                if (depth < 5) return  attenuation * GridWalk(scattered, depth + 1, currentX, currentY, currentZ);// +0.66 * GridWalk(Ray(tmprec.p, ray.direction), depth + 1, currentX, currentY, currentZ);
                else return attenuation;
                //else return skybox(ray);
            }
            
            //
        }




        
        //Take the step to next voxel
        tPrevious = tCurrent;
        tCurrent = tNext;
        currentX = nextX;
        currentY = nextY;
        currentZ = nextZ;

        
    }

    return skybox(ray);

}



