/********************************
* Documentation for defining lights
********************************/

#ifndef LIGHT_H
#define LIGHT_H

#include "Eigen\Dense"
#include "Color.h"
#include "Point.h"
#include "ray.h"

class Light;

/**
* All lights have a color asociated with them
*/
class Light{
public:
	Color* color;
	/*
	* Retrieves the direction vector from the pixel location to the light.
	*/
	virtual Vector3f* getDirection(Point* pixel_loc) = 0;

	/**
	 * Generates the light ray
	 */
	virtual Ray* generateRay(Point* pixel_loc) = 0;
};

/**
* Direction lights have a direction
*/
class DirectionLight : public Light {
public:
	Vector3f* direction;

	DirectionLight(float x, float y, float z, float r, float g, float b) {
		direction = new Vector3f(x, y, z);
		direction->normalize();
		*direction *= -1;
		color = new Color(r, g, b);
	}

	/*
	* Simply return the opposite direction, normalized
	*/
	Vector3f* getDirection(Point* pixel_loc) {
		return direction;
	};

	Ray* generateRay(Point* pixel_loc){
		return new Ray(new Point(0.0, 0.0, 0.0), direction, -INFINITY, INFINITY);
	};
};

/**
* Point lights have a location
*/
class PointLight : public Light{
public:
	Point* location;

	PointLight(float x, float y, float z, float r, float g, float b) {
		location = new Point(x, y, z);
		color = new Color(r, g, b);
	}

	/*
	* Returns pixel_loc - location, normalized
	*/
	Vector3f* getDirection(Point* pixel_loc) {
		Vector3f* result = new Vector3f(0, 0, 0);
		*result += *location - *pixel_loc;
		result->normalize();
		return result;
	}

	Ray* generateRay(Point* pixel_loc){
		return new Ray(location, getDirection(pixel_loc), 0, INFINITY);
	};

};

#endif