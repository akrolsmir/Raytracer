/**
 * Shape class and subclass
 */

#ifndef SHAPE_H
#define SHAPE_H

#include "ray.h"
#include "local.h"

/**
 * A drawable shape
 */
class Shape{
public:
	/**
	 * Tests if ray intersects with me. If so, assign t_hit, the
	 * intersection point, and the normal at that point, and return true.
	 * Else, return false.
	 */
	virtual bool intersect(Ray* ray, float* t_hit, Local* local);

	/**
	 * Returns if the ray intersects with me.
	 */
	virtual bool intersect(Ray* ray);
};

/**
 * A sphere. Ellipsoids will be supported through transformations
 */
class Sphere : public Shape{
public:
	/**
	 * Construct the unit sphere at the origin
	 */
	Sphere();

	/**
	 * Constructs the sphere centered at c with radius r
	 */
	Sphere(Point c, float r);
	
	bool intersect(Ray* ray, float* t_hit, Local* local);

	bool intersect(Ray* ray);

private:
	Point center;
	float radius;
};

/**
 * A triangle
 */
class Triangle : public Shape{
public:

	/**
	 * Constructs a right triangle in 2-space with
	 * vertices at (0,0), (1,0), (0,1)
	 */
	Triangle();

	/**
	 * Constructs the triangle with vertices a, b, c
	 */
	Triangle(Point a, Point b, Point c);

	bool intersect(Ray* ray, float* t_hit, Local* local);

	bool intersect(Ray* ray);

private:
	Point a;
	Point b;
	Point c;
};

#endif