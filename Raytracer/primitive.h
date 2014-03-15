/**
 * Class for primitives
 */

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "shape.h"
#include "transform.h"
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
	virtual bool intersect(Ray ray, float* t_hit, Intersection* in);
	/**
	 * Returns if the ray intersects with me.
	 */
	virtual bool intersect(Ray ray);

	virtual void addTransform(vector<Transformation*> trans, vector<Transformation*> invTrans);

	/**
	 * Returns my brdf
	 */
	virtual BRDF getBRDF();

	virtual BRDF* getBRDFPointer();

	virtual void setkd(Color kd);
	virtual void setks(Color ks);
	virtual void setka(Color ka);
	virtual void setsp(float sp);

};

/**
* Wrapper class containing the local geometry of a point
* on a primitive where a ray intersects the primitive
*/
class Intersection{
public:
	Local local;
	Primitive* primitive;

	/**
	* Sets loc to a default local and primitive to NULL
	*/
	Intersection();

	/**
	* Initializes loc and primitive
	*/
	Intersection(Local loc, Primitive* primitive);
};

/**
 * A geometric primitive consists of a shape and
 * the material properties of the shape
 */
class GeometricPrimitive : public Primitive{
private:
	Shape* shape;
	BRDF brdf;
public:

	vector<Transformation*> objToWorld;
	vector<Transformation*> worldToObj;

	/**
	 * Constructor
	 */
	GeometricPrimitive(Shape* shape, BRDF brdf);

	Vector3f getCenter();
	Vector3f getMinBB();
	Vector3f getMaxBB();
	bool intersect(Ray ray, float* t_hit, Intersection* in);
	bool intersect(Ray ray);
	void addTransform(vector<Transformation*> trans, vector<Transformation*> invTrans);
	BRDF getBRDF();
	BRDF* getBRDFPointer();
	void setkd(Color kd);
	void setks(Color ks);
	void setka(Color ka);
	void setsp(float sp);
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

	bool intersect(Ray ray, float* t_hit, Intersection* in);
	bool intersect(Ray ray);
	void addTransform(vector<Transformation*> trans, vector<Transformation*> invTrans);
	void addPrimitive(Primitive* p);
	BRDF getBRDF();
	BRDF* getBRDFPointer();
	void setkd(Color kd);
	void setks(Color ks);
	void setka(Color ka);
	void setsp(float sp);

private:
	std::vector<Primitive*> primitives;
};

#endif