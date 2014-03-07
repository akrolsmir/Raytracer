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
	Vector3f* direction = ray->getDir();
	Point* pos = ray->getPos();
	float result = sqrt(direction->dot((*pos - center)));
	//TODO
	delete pos;
	delete direction;
	return false;
}

bool Sphere::intersect(Ray* ray, float* t_hit, Local* local){
	//TODO
	return false;
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

