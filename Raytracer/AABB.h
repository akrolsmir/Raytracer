#ifndef AABB_H
#define AABB_H

#include "primitive.h"

class AABB {
public:
	AABB* left;
	AABB* right;
	AABB* parent;
	Vector3f minimum;
	Vector3f maximum;
	bool intersect(Ray ray, float* t_hit, Intersection* in);
	bool intersect(Ray ray);
	AABB(vector<GeometricPrimitive*> lst);
	AABB(AABB* a, AABB* b);
	AABB();
};

class AABB_Leaf : public AABB{
public:
	AABB* left;
	AABB* right;
	AABB* parent;
	Vector3f minimum;
	Vector3f maximum;
	AABB_Leaf(GeometricPrimitive* p);
	bool intersect(Ray ray, float* t_hit, Intersection* in);
	bool intersect(Ray ray);
private:
	GeometricPrimitive* p;
};

#endif