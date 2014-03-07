#include "point.h"

Point::Point() : 
	Vector3f(){ /*Nothing*/ }

Point::Point(Vector3f* p){
	this->operator()(0) = p->operator()(0);
	this->operator()(1) = p->operator()(1);
	this->operator()(2) = p->operator()(2);
}

void Point::setLoc(float x, float y, float z){
	this->operator()(0) = x;
	this->operator()(1) = y;
	this->operator()(2) = z;
}

void Point::setLoc(Vector3f* loc){
	this->operator()(0) = loc->operator()(0);
	this->operator()(1) = loc->operator()(1);
	this->operator()(2) = loc->operator()(2);
}

void Point::setX(float x){
	this->operator()(0) = x;
}

void Point::setY(float y){
	this->operator()(1) = y;
}

void Point::setZ(float z){
	this->operator()(2) = z;
}

float Point::getX(){
	return this->operator()(0);
}

float Point::getY(){
	return this->operator()(1);
}

float Point::getZ(){
	return this->operator()(2);
}

Vector3f* Point::getLoc(){
	return new Vector3f(this->getX(), this->getY(), this->getZ());
}
