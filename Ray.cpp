#include "Ray.h"

Ray::Ray(const Vec3& org, const Vec3& dir) :
	origin(org), direction(unit_vector(dir)),
	directionInverse(1.0f / direction.x(), 1.0f / direction.y(), 1.0f / direction.z())
{}

Vec3 Ray::at(float t) const {
	return origin + direction*t;
}


Ray Ray::transform(const Matrix4& transMatrix) const {
	Vec3 newOrigin = transMatrix.multiplyVector(origin);
	Vec3 newDirection = transMatrix.multiplyVector(direction);
	return Ray(newOrigin, newDirection);
}