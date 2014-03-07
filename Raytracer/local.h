/**
 * Class describing the local geometry on a primitive
 */

#ifndef LOCAL_H
#define LOCAL_H

#include "point.h"

class Local{
public:

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
	 * Returns a copy of the normal at p. Is 0 if
	 * p is not on the boundary of some primitive.
	 */
	Vector3f* getNormal();

private:
	Point pos;
	Vector3f normal; //normalized vector.
};

#endif