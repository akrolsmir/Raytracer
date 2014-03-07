/**
 * Point class
 */

#ifndef POINT_H
#define POINT_H

#include "Eigen/Dense"

using namespace Eigen;

class Point : public Vector3f{
public:

	/**
	 * Set the point's coordinate's to x,y,z
	 */
	void setLoc(float x, float y, float z);

	/**
	 * Set the point's coordinate's to loc
	 */
	void setLoc(Vector3f loc);

	/**
	 * Set my X value to x
	 */
	void setX(float x);

	/**
	 * Set my Y value to y
	 */
	void setY(float y);

	/**
	 * Set my Z value to z
	 */
	void setZ(float z);

	/**
	 * Gets my X value
	 */
	float getX();

	/**
	 * Gets my Y Value
	 */
	float getY();

	/**
	 * Gets my Z value
	 */
	float getZ();

	/**
	 * Returns a pointer to a copy of my location
	 */
	Vector3f* getLoc();
};

#endif