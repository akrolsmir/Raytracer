/**
 * Shape class and subclass
 */

#ifndef SHAPE_H
#define SHAPE_H

#include "basic.h"

/**
 * A drawable shape
 */
class Shape{
public:
	/**
	 * Tests if ray intersects with me. If so, assign t_hit, the
	 * intersection point, and the normal at that point, and return true.
	 * Else, return false.
	 */
	virtual bool intersect(Ray ray, float* t_hit, Local* local);

	/**
	 * Returns if the ray intersects with me.
	 */
	virtual bool intersect(Ray ray);

	virtual Vector3f getCenter();
	virtual Vector3f getMinBB();
	virtual Vector3f getMaxBB();
	virtual void setCenter(Point c){};
};

/**
 * A sphere. Ellipsoids will be supported through transformations
 */
class Sphere : public Shape{
public:
	/**
	 * Construct the unit sphere at the origin
	 */
	Sphere();

	/**
	 * Constructs the sphere centered at c with radius r
	 */
	Sphere(Point c, float r);
	
	bool intersect(Ray ray, float* t_hit, Local* local);

	bool intersect(Ray ray);

	Vector3f getCenter();
	Vector3f getMinBB();
	Vector3f getMaxBB();
	void setCenter(Point c){
		center = c;
	}

private:
	Point center;
	float radius;
};

/**
 * A triangle
 */
class Triangle : public Shape{
public:

	/**
	 * Constructs a right triangle in 2-space with
	 * vertices at (0,0), (1,0), (0,1)
	 */
	Triangle();

	/**
	 * Constructs the triangle with vertices a, b, c
	 */
	Triangle(Point a, Point b, Point c);

	bool intersect(Ray ray, float* t_hit, Local* local);

	bool intersect(Ray ray);

	Vector3f getCenter();
	Vector3f getMinBB();
	Vector3f getMaxBB();

protected:
	Vector3f getNormal(Point loc);
	Point a;
	Point b;
	Point c;

private:
	Vector3f normal;
};

/**
 * A triangle with specified normal vectors
 */
class NormalTriangle : public Triangle{
public:
	NormalTriangle(Point a, Point b, Point c, Vector3f an, Vector3f bn, Vector3f cn);
	bool intersect(Ray ray, float* t_hit, Local* local);
	Vector3f getMinBB();
	Vector3f getMaxBB();

protected:
	Vector3f getNormal(Point loc);
	Point a;
	Point b;
	Point c;
private:
	Point an;
	Point bn;
	Point cn;
};

class Parallelogram : public Shape {
public:
	Parallelogram(Point a, Point b, Point c) :
		a(a), b(b), c(c){/*nothing*/
	}

	bool intersect(Ray ray){
		//UNIMPLEMENTED
		exit(0);
	}
	bool intersect(Ray ray, float* t_hit, Local* local){
		//UNIMPLEMENTED
		exit(0);
	}

	bool intersect(Ray ray, float* be, float* ga){
		Matrix3f M;
		Vector3f rayStart = ray.pos;
		Vector3f rayDirection = ray.dir;
		Vector3f ab = a - b;
		Vector3f ac = a - c;
		Vector3f aStart = a - rayStart;
		M << ab, ac, rayDirection;
		float MDet = M.determinant();
		if (MDet == 0) {
			return false;
		}
		Matrix3f tMatrix;
		tMatrix << ab, ac, aStart;
		float t = tMatrix.determinant() / MDet;
		if (!ray.inBounds(t)){
			return false;
		}
		Matrix3f gammaMatrix;
		gammaMatrix << ab, aStart, rayDirection;
		float gamma = gammaMatrix.determinant() / MDet;
		if (gamma < 0 || gamma > 1){
			return false;
		}

		Matrix3f betaMatrix;
		betaMatrix << aStart, ac, rayDirection;
		float beta = betaMatrix.determinant() / MDet;
		if (beta < 0 || beta > 1){
			return false;
		}

		*be = beta;
		*ga = gamma;
		return true;
	}
private:
	Point a;
	Point b;
	Point c;
};

#endif