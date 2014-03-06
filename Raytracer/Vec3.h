/**
* Wrapper class for a 3-element vector
*/

#ifndef VEC3_H
#define VEC3_H

#include "GraphVector.h"

class Vec3 : public Vec<3>{
public:
	/*
	* Initializes with (x, y, z)
	*/
	Vec3(float x, float y, float z);

	Vec3();

	/*
	* updates me to be my cross prodcut with x
	*/
	void crossProd(Vec3* x);
};

/**
* Returns x + y
*/
Vec3* add(Vec3* x, Vec3* y);

/**
* Returns x - y
*/
Vec3* sub(Vec3* x, Vec3* y);

/**
* Returns the dot product of x and y
*/
float dotProd(Vec3* x, Vec3* y);

/**
* Returns x cross y
*/
Vec3* crossProd(Vec3* x, Vec3* y);;

/**
* Returns the product of x and y
*/
Vec3* scalar_mult(float x, Vec3* y);

/**
* Returns the pointwise multipication of x and y
*/
Vec3* point_mult(Vec3* x, Vec3* y);

/**
* Returns a pointer to a vector in the same direction as x with magnitude = 1
*/
Vec3* cp_normal(Vec3* x);

#endif