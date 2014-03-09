/**
 * Class for primitives
 */

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "ray.h"
#include "shape.h"
#include "shader.h"
#include "intersection.h"
#include <vector>

class Intersection;

/**
 * Abstract class for primtives
 */
class Primitive{
public:
	/**
	 * Returns if the ray intersections with me. If the ray intersects with me,
	 * sets t_hit to the t such that ray(t) is on the surface of the primitive,
	 * and the intersection containing the primitive and the local geometry
	 */
	virtual bool intersect(Ray* ray, float* t_hit, Intersection* in);
	/**
	 * Returns if the ray intersects with me.
	 */
	virtual bool intersect(Ray* ray);
	/**
	 * Returns my brdf
	 */
	virtual BRDF getBRDF();

};

/**
 * A geometric primitive consists of a shape and
 * the material properties of the shape
 */
class GeometricPrimitive : public Primitive{
private:
	Shape* shape;
	BRDF brdf;
	const Matrix4f objToWorld;
	const Matrix4f worldToObj;
public:
	/**
	 * Constructor
	 */
	GeometricPrimitive(Shape* shape, BRDF brdf);

	bool intersect(Ray* ray, float* t_hit, Intersection* in);
	bool intersect(Ray* ray);
	BRDF getBRDF();
};

/**
 * An aggregate primitive is a collection of primitives
 */
class AggregatePrimitive : public Primitive{
public:
	/**
	 * Constructors
	 */
	AggregatePrimitive();
	AggregatePrimitive(std::vector<Primitive*> lst);

	bool intersect(Ray* ray, float* t_hit, Intersection* in);
	bool intersect(Ray* ray);
	BRDF getBRDF();

private:
	std::vector<Primitive*> primitives;
};

#endif