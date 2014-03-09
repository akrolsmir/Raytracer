#include "ray.h"
#include "limits.h"

Ray::Ray() : 
	pos(Point()), dir(Vector3f()), t_min(0), t_max(FLT_MAX){ /*nothing*/ }

Ray::Ray(Point p, Vector3f direction, float tMin, float tMax) : 
	pos(p), dir(direction), t_min(tMin), t_max(tMax){/*nothing*/}

Point* Ray::calculatePosition(float t){
	if (t < t_min || t > t_max){
		return NULL;
	}
	Vector3f result = pos + t*dir;
	return new Point(result);
}
