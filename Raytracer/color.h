/**
* Color class
*/

#ifndef COLOR_H
#define COLOR_H

#include "Eigen/Dense"

using namespace Eigen;

class Color : public Vector3f{
public:

	/**
	 * Default constructor same as superclass
	 */
	Color();

	/**
	* Same constructor as Vector3f
	*/
	Color(float a, float b, float c);

	/**
	* Initializes from values from col
	*/
	Color(Vector3f col);

	/**
	* Set the color's RGB values to r,g,b.
	*/
	void setRGB(float r, float g, float b);

	/**
	* Set the colors's RGB values to rgb
	*/
	void setRGB(Vector3f* rgb);

	/**
	* Set my red value to r
	*/
	void setR(float r);

	/**
	* Set my green value to g
	*/
	void setG(float g);

	/**
	* Set my blue value to b
	*/
	void setB(float b);

	/**
	* Gets my red value
	*/
	float getR();

	/**
	* Gets my green Value
	*/
	float getG();

	/**
	* Gets my blue value
	*/
	float getB();

	/**
	* Returns a pointer to a copy of my rgb values
	*/
	Vector3f* getRGB();

};

#endif