/**
 * Class describing the local geometry on a primitive
 */

#ifndef LOCAL_H
#define LOCAL_H

#include "point.h"

/**
 * A local geometry is composed of a point on the
 * surface of an object and the normal vector to
 * the surface. 
 */
class Local{
public:

	/**
	 * Creates a local at the origin with a 0 normal vector
	 */
	Local();

	/**
	 * Initializes the local geometry at point
	 * p with normal vector norm
	 */
	Local(Point* p, Vector3f* norm);

	/**
	 * Returns a copy of the location
	 */
	Point* getPoint();

	/**
	 * Returns a copy of the normal at p, assuming that
	 * this is a valid local geometry
	 */
	Vector3f* getNormal();

	Point pos;
	Vector3f normal; //normalized vector.
};

#endif