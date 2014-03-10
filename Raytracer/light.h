/********************************
* Documentation for defining lights
********************************/

#ifndef LIGHT_H
#define LIGHT_H

#include "Eigen\Dense"
#include "basic.h"

class Light;

/**
* All lights have a color asociated with them
*/
class Light{
public:
	Color color;
	/*
	* Retrieves the direction vector from the pixel location to the light.
	*/
	virtual Vector3f* getDirection(Point pixel_loc) = 0;

	/**
	 * Generates the light ray
	 */
	virtual Ray* generateRay(Point pixel_loc) = 0;
};

/**
* Direction lights have a direction
*/
class DirectionLight : public Light {
public:
	Vector3f direction;

	DirectionLight(float x, float y, float z, float r, float g, float b) {
		direction = Vector3f(x, y, z);
		direction.normalize();
		direction *= -1;
		color = Color(r, g, b);
	}

	/*
	* Simply return the opposite direction, normalized
	*/
	Vector3f* getDirection(Point pixel_loc) {
		return &direction;
	};

	Ray* generateRay(Point pixel_loc){
		return new Ray(Point(0.0, 0.0, 0.0), direction, -INFINITY, INFINITY);
	};
};

/**
* Point lights have a location
*/
class PointLight : public Light{
public:
	Point location;

	PointLight(float x, float y, float z, float r, float g, float b) {
		location = Point(x, y, z);
		color = Color(r, g, b);
	}

	/*
	* Returns location - pixel_loc, normalized
	*/
	Vector3f* getDirection(Point pixel_loc) {
		Vector3f* result = new Vector3f(0, 0, 0);
		*result += location - pixel_loc;
		result->normalize();
		return result;
	}

	Ray* generateRay(Point pixel_loc){
		Point dir = location - pixel_loc;
		return new Ray(pixel_loc, dir, 0, 1);
	};

};

#endif