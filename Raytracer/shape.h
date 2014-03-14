/**
 * Shape class and subclass
 */

#ifndef SHAPE_H
#define SHAPE_H

#include "basic.h"

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
	virtual bool intersect(Ray ray, float* t_hit, Local* local);

	/**
	 * Returns if the ray intersects with me.
	 */
	virtual bool intersect(Ray ray);
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
	
	bool intersect(Ray ray, float* t_hit, Local* local);

	bool intersect(Ray ray);

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

	bool intersect(Ray ray, float* t_hit, Local* local);

	bool intersect(Ray ray);

protected:
	Vector3f getNormal(Point loc);
	Point a;
	Point b;
	Point c;

private:
	Vector3f normal;
};

/**
 * A triangle with specified normal vectors
 */
class NormalTriangle : public Triangle{
public:
	NormalTriangle(Point a, Point b, Point c, Vector3f an, Vector3f bn, Vector3f cn);
	bool intersect(Ray ray, float* t_hit, Local* local);

protected:
	Vector3f getNormal(Point loc);
	Point a;
	Point b;
	Point c;
private:
	Point an;
	Point bn;
	Point cn;
};

#endif