#ifndef VOXEL_H
#define VOXEL_H

#include "AABB.h"
#include "hittable.h"
//#include "Hittable_List.h"
#include <vector>

#include "Material.h"

class Voxel : public AABB
{
	public:
	
	bool occupied;

	

	opticalMedium* material;
	//Hittable_List primitives;
	std::vector<Hittable*> primitives;

	Voxel();
	Voxel(opticalMedium* matPtr);
	Voxel(const Vec3& minPoint, const Vec3& maxPoint, opticalMedium* matPtr);

	bool primitivesHit(const Ray& r, float tmin, float tmax, hit_record& rec);

};


#endif

