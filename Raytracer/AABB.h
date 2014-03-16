#ifndef AABB_H
#define AABB_H

#include "primitive.h"


class AABB_Tree {
public:
	int d_from_root;
	AABB_Tree* left;
	AABB_Tree* right;
	AABB_Tree* parent;
	Vector3f minimum;
	Vector3f maximum;
	virtual bool intersect(Ray ray, float* t_hit, Intersection* in);
	virtual bool intersect(Ray ray);
	AABB_Tree();
};

class AABB : public AABB_Tree{
public:
	/*
	int d_from_root;
	AABB_Tree* left;
	AABB_Tree* right;
	AABB_Tree* parent;
	Vector3f minimum;
	Vector3f maximum;
	*/
	bool intersect(Ray ray, float* t_hit, Intersection* in);
	bool intersect(Ray ray);
	AABB(vector<GeometricPrimitive*>& lst);
	AABB(AABB* a, AABB* b);
};

class AABB_Leaf : public AABB_Tree{
public:
	/*
	AABB* left;
	AABB* right;
	AABB* parent;
	Vector3f minimum;
	Vector3f maximum;
	*/
	AABB_Leaf(GeometricPrimitive* p);
	bool intersect(Ray ray, float* t_hit, Intersection* in);
	bool intersect(Ray ray);
private:
	GeometricPrimitive* p;
};

#endif