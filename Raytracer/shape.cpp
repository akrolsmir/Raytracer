#include "shape.h"
#include <iostream>
#include <cmath>

#define UNIMPLEMENTED(str) std::cout << "Unimplemented method " << str << std::endl; exit(0);

bool Shape::intersect(Ray* ray){
	UNIMPLEMENTED("Intersect");
}

bool Shape::intersect(Ray* ray, float* t_hit, Local* local){
	UNIMPLEMENTED("Intersect");
}

Sphere::Sphere() :
	center(Point(0.0, 0.0, 0.0)), radius(1.0){/*nothing*/}

Sphere::Sphere(Point* c, float r) :
	center(*c), radius(r){/*nothing*/}

bool Sphere::intersect(Ray* ray){
	Vector3f d = *ray->getDir();
	Point e = *ray->getPos() - center;
	float disc = d.dot(e) * d.dot(e) - d.dot(d) * (e.dot(e) - radius * radius);
	return disc >= 0;
}

bool Sphere::intersect(Ray* ray, float* t_hit, Local* local){
	Vector3f d = *ray->getDir();
	Point e = *ray->getPos() - center;
	float disc = d.dot(e) * d.dot(e) - d.dot(d) * (e.dot(e) - radius * radius);
	if (disc < 0) return false;
	if (d.dot(d) <= 0) return false;
	
	*t_hit = (-e.dot(d) - sqrt(disc)) / d.dot(d);
	if (ray->calculatePosition(*t_hit) == NULL) {
		*t_hit = (-e.dot(d) + sqrt(disc)) / d.dot(d);
		if (ray->calculatePosition(*t_hit) == NULL) return false;
	}

	local->pos = *ray->calculatePosition(*t_hit);
	local->normal = (local->pos - center).normalized();

	return true;
}

Triangle::Triangle() :
	a(Point(0.0,0.0,0.0)), b(Point(1.0,0.0,0.0)), c(Point(0.0,1.0,0.0)){/*nothing*/}

Triangle::Triangle(Point* a, Point* b, Point* c) :
	a(*a), b(*b), c(*c){/*nothing*/}

bool Triangle::intersect(Ray* ray){
	Matrix3f M;
	Vector3f rayStart = *(ray->getPos());
	Vector3f rayDirection = *(ray->getDir());
	M << (a - b), (a - c), rayDirection;
	float MDet = M.determinant();
	if (MDet == 0) {
		return false;
	}
	Matrix3f tMatrix;
	tMatrix << (a - b), (a - c), (a - rayStart);
	float t = tMatrix.determinant() / MDet;
	if (t < ray->getTMin() || t > ray->getTMax()){
		return false;
	}
	Matrix3f gammaMatrix;
	gammaMatrix << (a - b), (a - rayStart), rayDirection;
	float gamma = gammaMatrix.determinant() / MDet;
	if (gamma < 0 || gamma > 1){
		return false;
	}

	Matrix3f betaMatrix;
	betaMatrix << (a - rayStart), a - c, rayDirection;
	float beta = betaMatrix.determinant() / MDet;
	if (beta < 0 || beta > 1 - gamma){
		return false;
	}

	return true;
}

bool Triangle::intersect(Ray* ray, float* t_hit, Local* local){
	Matrix3f M;
	Vector3f rayStart = *(ray->getPos());
	Vector3f rayDirection = *(ray->getDir());
	M << (a - b), (a - c), rayDirection;
	float MDet = M.determinant();
	if (MDet == 0) {
		return false;
	}
	Matrix3f tMatrix;
	tMatrix << (a - b), (a - c), (a - rayStart);
	float t = tMatrix.determinant() / MDet;
	if (t < ray->getTMin() || t > ray->getTMax()){
		return false;
	}
	Matrix3f gammaMatrix;
	gammaMatrix << (a - b), (a - rayStart), rayDirection;
	float gamma = gammaMatrix.determinant() / MDet;
	if (gamma < 0 || gamma > 1){
		return false;
	}

	Matrix3f betaMatrix;
	betaMatrix << (a - rayStart), a - c, rayDirection;
	float beta = betaMatrix.determinant() / MDet;
	if (beta < 0 || beta > 1 - gamma){
		return false;
	}

	*t_hit = t;

	local->pos = *ray->calculatePosition(t);
	local->normal = (b - a).cross(c - a).normalized();
	//std::cout << local->pos << std::endl << std::endl;
	return true;


}

