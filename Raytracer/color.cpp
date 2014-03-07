#include "color.h"

void Color::setRGB(float r, float g, float b){
	this->operator()(0) = r;
	this->operator()(1) = g;
	this->operator()(2) = b;
}

void Color::setRGB(Vector3f loc){
	this->operator()(0) = loc(0);
	this->operator()(1) = loc(1);
	this->operator()(2) = loc(2);
}

void Color::setR(float r){
	this->operator()(0) = r;
}

void Color::setG(float g){
	this->operator()(1) = g;
}

void Color::setB(float b){
	this->operator()(2) = b;
}

float Color::getR(){
	return this->operator()(0);
}

float Color::getG(){
	return this->operator()(1);
}

float Color::getB(){
	return this->operator()(2);
}

Vector3f* Color::getRGB(){
	return new Vector3f(this->getR(), this->getG(), this->getB());
}
