#include "Box.h"
#include <vector>

Box::Box(Vec3 min, Vec3 max, int rotationX, int rotationY, int rotationZ, Material* m) : minPoint(min), maxPoint(max), Hittable(m){
	
	if (rotationX % 90 == 0 && rotationY % 90 == 0 && rotationZ % 90 == 0) {
		//there is no rotation, simple Axis-Aligned Box;


		this->rotDegX = 0;
		this->rotDegY = 0;
		this->rotDegZ = 0;
	}
	else {
		this->rotDegX = rotationX % 90;
		this->rotDegY = rotationY % 90;
		this->rotDegZ = rotationZ % 90;
	}

	this->minPoint = min;
	this->maxPoint = max;

	this->rotationMatrix.calculateMatrix(rotDegX, rotDegY, rotDegZ);
	this->rotationMatrixInverse = rotationMatrix;
	setAABB();
	this->rotationMatrixInverse.invert();

	
}

void Box::setAABB() {
	//Calculate all 8 vertices, rotate all of them and find the new min/max for the BB


	Vec3 center = this->minPoint + (this->maxPoint - this->minPoint) / 2;

	this->minPoint = this->minPoint - center;
	this->maxPoint = this->maxPoint - center;

	std::vector<Vec3> vertices;

	vertices.push_back(Vec3(this->minPoint.x(), this->minPoint.y(), this->minPoint.z()));
	vertices.push_back(Vec3(this->minPoint.x(), this->minPoint.y(), this->maxPoint.z()));

	vertices.push_back(Vec3(this->minPoint.x(), this->maxPoint.y(), this->minPoint.z()));
	vertices.push_back(Vec3(this->minPoint.x(), this->maxPoint.y(), this->maxPoint.z()));

	vertices.push_back(Vec3(this->maxPoint.x(), this->minPoint.y(), this->minPoint.z()));
	vertices.push_back(Vec3(this->maxPoint.x(), this->minPoint.y(), this->maxPoint.z()));

	vertices.push_back(Vec3(this->maxPoint.x(), this->maxPoint.y(), this->minPoint.z()));
	vertices.push_back(Vec3(this->maxPoint.x(), this->maxPoint.y(), this->maxPoint.z()));

	float minX = vertices[0].x();
	float minY = vertices[0].y();
	float minZ = vertices[0].z();

	float maxX = vertices[0].x();
	float maxY = vertices[0].y();
	float maxZ = vertices[0].z();

	for (Vec3 v : vertices) {
		v = this->rotationMatrix.multiplyVector(v);

		if (v.x() < minX) minX = v.x();
		if (v.y() < minY) minY = v.y();
		if (v.z() < minZ) minZ = v.z();
		if (v.x() > maxX) maxX = v.x();
		if (v.y() > maxX) maxY = v.y();
		if (v.z() > maxZ) maxZ = v.z();
	}



	this->boundingBox.minPoint.set(minX+center.x(), minY + center.y(), minZ + center.z());
	this->boundingBox.maxPoint.set(maxX + center.x(), maxY + center.y(), maxZ + center.z());

	this->minPoint = this->minPoint + center;
	this->maxPoint = this->maxPoint + center;
}

bool Box::hit(const Ray& r, float tmin, float tmax, hit_record& rec) const {
	if (this->rotDegX == 0 && rotDegY == 0 && rotDegZ == 0) {
		//no rotation. AABB was hit so just return the material pointer;
		rec.mat_ptr = this->mat_ptr;
		return true;
	}

	Ray localRay = r.transform(this->rotationMatrixInverse);

	AABB transformedBox(this->minPoint, this->maxPoint);

	bool b = transformedBox.intersect(localRay, tmin, tmax, rec);
	if (!b) return false;

	rec.normal = this->rotationMatrix.multiplyVector(rec.normal);
	Vec3 tmp1 = this->rotationMatrix.multiplyVector(rec.p);
	Vec3 tmp2 = this->rotationMatrixInverse.multiplyVector(rec.p);
	Vec3 tmp3 = r.at(rec.t);
	Vec3 tmp4 = localRay.at(rec.t);
	rec.p = r.at(rec.t);
	rec.mat_ptr = this->mat_ptr;

	return true;

}

bool Box::isPointInside(Vec3 point) {
	return boundingBox.isPointInside(point);
}