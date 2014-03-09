/**
 * Class for representing light rays
 */

#ifndef RAY_H
#define RAY_H

#include "point.h"

/**
 * A ray is represented by the parametric equation
 * ray(t) = pos + t*dir, where t_min <= t <= t_max
 */
class Ray{
public:
	/**
	 * Default constructor
	 */
	Ray();

	/**
	 * Constructs a ray based on its starting point, a direction, 
	 * and a range tMin <= t <= tMax
	 */
	Ray(Point* p, Vector3f* direction, float tMin, float tMax);

	/**
	 * Calcluates ray(t)
	 * If t < t_min or t > t_max, returns NULL
	 */
	Point* calculatePosition(float t);

	/**
	 * Returns a copy of the position
	 */
	Point* getPos();

	/**
	 * Returns a copy of the direction
	 */
	Vector3f* getDir();

	/**
	 * Returns t_min
	 */
	float getTMin();

	/**
	 * Returns t_max
	 */
	float getTMax();

private:
	Point pos;
	Vector3f dir;
	float t_min, t_max;
};

#endif