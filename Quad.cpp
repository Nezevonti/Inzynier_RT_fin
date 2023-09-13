#include "Quad.h"

Quad::Quad(Vec3 minPoint, Vec3 u, Vec3 v, Material* m) : minPoint(minPoint), u(u), v(v), Hittable(m) {
	Vec3 uvCross = cross(u, v);
	this->normal = unit_vector(uvCross);

	w = uvCross / uvCross.squared_length();

	setAABB();

	float tmp;
	//check if there is no flipped coords between min and max
	if (this->boundingBox.minPoint.x() > this->boundingBox.maxPoint.x()) {
		std::swap(this->boundingBox.minPoint.e[0], this->boundingBox.maxPoint.e[0]);
	}
	if (this->boundingBox.minPoint.y() > this->boundingBox.maxPoint.y()) {
		std::swap(this->boundingBox.minPoint.e[1], this->boundingBox.maxPoint.e[1]);
	}
	if (this->boundingBox.minPoint.z() > this->boundingBox.maxPoint.z()) {
		std::swap(this->boundingBox.minPoint.e[2], this->boundingBox.maxPoint.e[2]);
	}
	this->boundingBox.padBB();

}

bool Quad::hit(const Ray& r, float tmin, float tmax, hit_record& rec) const {
	float normalDotDirection = dot(this->normal, r.direction);

	//check if the ray is parallel to the plane
	if (std::abs(normalDotDirection) <= FLT_EPSILON) {
		return false;
	}

	// is t between tmin & tmax?
	float localT = (dot(normal, this->minPoint) - dot(normal, r.origin)) / normalDotDirection;
	if (localT < tmin || localT > tmax)	return false;

	//is the point somewhere inside the quad?
	Vec3 intersectionPoint = r.at(localT);

	Vec3 planeIntersection = intersectionPoint - this->minPoint;
	float a = dot(w, cross(planeIntersection, v));
	float b = dot(w, cross(u, planeIntersection));

	if (!insideQuad(a, b, rec)) {
		return false;
	}

	rec.t = localT;
	rec.p = intersectionPoint;

	rec.mat_ptr = this->mat_ptr;
	rec.normal = this->normal;
}

bool Quad::insideQuad(float a, float b, hit_record& rec) const {
	
	if ((a < 0) || (1 < a) || (b < 0) || (1 < b))
		return false;

	rec.u = a;
	rec.v = b;
	return true;

}


void Quad::setAABB() {
	boundingBox.minPoint = minPoint;
	boundingBox.maxPoint = minPoint + u + v;
}

bool Quad::isPointInside(Vec3 point) {
	return false;
}