#include "color.h"

Color::Color() : 
	Vector3f(){/*nothing*/}

Color::Color(float a, float b, float c) :
	Vector3f(a, b, c){/* nothing */}

Color::Color(Vector3f col){
	*this = col;
}

void Color::setRGB(float r, float g, float b){
	this->operator()(0) = r;
	this->operator()(1) = g;
	this->operator()(2) = b;
}

void Color::setRGB(Vector3f* rgb){
	this->operator()(0) = rgb->operator()(0);
	this->operator()(1) = rgb->operator()(1);
	this->operator()(2) = rgb->operator()(2);
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
