#include "shape.h"
#include <iostream>
#include <cmath>

#define UNIMPLEMENTED(str) std::cout << "Unimplemented method " << str << std::endl; exit(0);

bool Shape::intersect(Ray ray){
	UNIMPLEMENTED("Intersect");
}

bool Shape::intersect(Ray ray, float* t_hit, Local* local){
	UNIMPLEMENTED("Intersect");
}

Sphere::Sphere() :
	center(Point(0.0, 0.0, 0.0)), radius(1.0){/*nothing*/}

Sphere::Sphere(Point c, float r) :
	center(c), radius(r){/*nothing*/}

bool Sphere::intersect(Ray ray){
	return intersect(ray, NULL, NULL);
}

bool Sphere::intersect(Ray ray, float* t_hit, Local* local){
	Vector3f d = ray.dir;
	Point e = ray.pos - center;
	float disc = d.dot(e) * d.dot(e) - d.dot(d) * (e.dot(e) - radius * radius);
	if (disc < 0) return false;
	if (d.dot(d) <= 0) return false;
	
	float t = (-e.dot(d) - sqrt(disc)) / d.dot(d);
	if (!ray.inBounds(t)) {
		t = (-e.dot(d) + sqrt(disc)) / d.dot(d);
		if (!ray.inBounds(t)) return false;
	}

	if (local && t_hit) {
		*t_hit = t;

		local->pos = ray.calculatePosition(t);
		local->normal = (local->pos - center).normalized();
	}
	return true;
}

Triangle::Triangle() :
	a(Point(0.0,0.0,0.0)), b(Point(1.0,0.0,0.0)), c(Point(0.0,1.0,0.0)), normal(0.0, 0.0, 1.0){/*nothing*/}

Triangle::Triangle(Point a, Point b, Point c) :
	a(a), b(b), c(c){
	normal = (b - a).cross(c - a).normalized();
}

bool Triangle::intersect(Ray ray){
	return intersect(ray, NULL, NULL);
}

bool Triangle::intersect(Ray ray, float* t_hit, Local* local){
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
	if (beta < 0 || beta > 1 - gamma){
		return false;
	}

	if (t_hit && local) {
		*t_hit = t;

		local->pos = ray.calculatePosition(t);
		local->normal = getNormal(Point(1 - beta - gamma, beta, gamma));
	}
	return true;
}

Vector3f Triangle::getNormal(Point loc){
	return normal;
}

NormalTriangle::NormalTriangle(Point a, Point b, Point c, Vector3f an, Vector3f bn, Vector3f cn) :
	a(a), b(b), c(c), an(an), bn(bn), cn(cn){
}

bool NormalTriangle::intersect(Ray ray, float* t_hit, Local* local){
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
	if (beta < 0 || beta > 1 - gamma){
		return false;
	}

	if (t_hit && local) {
		*t_hit = t;

		local->pos = ray.calculatePosition(t);
		local->normal = getNormal(Point(1 - beta - gamma, beta, gamma));
	}
	return true;
}

Vector3f NormalTriangle::getNormal(Point loc){
	return (loc(0) * an + loc(1) * bn + loc(2) * cn).normalized();
}