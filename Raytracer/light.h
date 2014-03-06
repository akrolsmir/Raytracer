/********************************
* Documentation for defining lights
********************************/

#ifndef LIGHT_H
#define LIGHT_H

#include "Vec3.h"

class Light;

/**
* All lights have a color asociated with them
*/
class Light{
public:
	Vec3* color;
	/*
	* Retrieves the direction vector from the pixel location to the light.
	*/
	virtual Vec3* getDirection(Vec3* pixel_loc) = 0;
};

/**
* Direction lights have a direction
*/
class DirectionLight : public Light {
public:
	Vec3* direction;

	DirectionLight(float x, float y, float z, float r, float g, float b) {
		direction = new Vec3(x, y, z);
		color = new Vec3(r, g, b);
	}

	/*
	* Simply return the opposite direction, normalized
	*/
	Vec3* getDirection(Vec3* pixel_loc) {
		Vec3* result = cp_normal(direction);
		result->scalar_mult(-1);
		return result;
	};
};

/**
* Point lights have a location
*/
class PointLight : public Light{
public:
	Vec3* location;

	PointLight(float x, float y, float z, float r, float g, float b) {
		location = new Vec3(x, y, z);
		color = new Vec3(r, g, b);
	}

	/*
	* Returns pixel_loc - location, normalized
	*/
	Vec3* getDirection(Vec3* pixel_loc) {
		Vec3* result = sub(location, pixel_loc);
		result->normalize();
		return result;
	};
};

#endif