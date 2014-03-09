
#ifndef INTERSECT_H
#define INTERSECT_H

#include "basic.h"
#include "primitive.h"

class Primitive;

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

#endif