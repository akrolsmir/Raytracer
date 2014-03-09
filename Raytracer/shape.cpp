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

// WHY WON"T THIS WORK????
bool Sphere::intersect(Ray* ray, float* t_hit, Local* local){
	Vector3f d = *ray->getDir();
	Point e = *ray->getPos() - center;
	float disc = d.dot(e) * d.dot(e) - d.dot(d) * (e.dot(e) - radius * radius);
	if (disc < 0) return false;

	std::cout << "D.D: " << d.dot(d) << std::endl;
	float temp = (-e.dot(d) - disc) / d.dot(d);
	std::cout << "T_HIT: " << *t_hit << std::endl;

	if (d.dot(d) <= 0.0001) return false;
	
	*t_hit = temp;
	
	std::cout << ray << "," << t_hit << "," << local << std::endl;

	//local->pos = ray->calculatePosition(*t_hit);
	//local->normal = &((*local->pos - center).normalized());

	return true;
}

Triangle::Triangle() :
	a(Point(0.0,0.0,0.0)), b(Point(1.0,0.0,0.0)), c(Point(0.0,1.0,0.0)){/*nothing*/}

Triangle::Triangle(Point* a, Point* b, Point* c) :
	a(*a), b(*b), c(*c){/*nothing*/}

bool Triangle::intersect(Ray* ray){
	//TODO
	return false;
}

bool Triangle::intersect(Ray* ray, float* t_hit, Local* local){
	//TODO
	return false;
}

