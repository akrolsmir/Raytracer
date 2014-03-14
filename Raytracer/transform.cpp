#include "transform.h"
#include <iostream>
#define UNIMPLEMENTED(str) std::cout << "Unimplemented method " << str << std::endl; exit(0);

Vector3f Transformation::applyTransformation(Vector3f val, int type){
	UNIMPLEMENTED("applyTransformation");
	return val;
}

Translate::Translate(float tx, float ty, float tz){
	mat = Matrix4f();
	mat(0, 0) = 1;
	mat(1, 1) = 1;
	mat(2, 2) = 1;
	mat(3, 3) = 1;
	mat(0, 3) = tx;
	mat(1, 3) = ty;
	mat(2, 3) = tz;
}

Vector3f Translate::applyTransformation(Vector3f val, int type){
	Vector4f temp;
	temp(0) = val(0);
	temp(1) = val(1);
	temp(2) = val(2);
	temp(3) = type;
	temp = mat*temp;
	val(0) = temp(0);
	val(1) = temp(1);
	val(2) = temp(2);
	return val;
}

Rotate::Rotate(Vector3f axis, double angle){
	Vector3f norm_axis = axis.normalized();
	Vector3f imag = norm_axis*sin(angle/2);
	double real = cos(angle / 2);
	quat = Quaternionf(imag(0), imag(1), imag(2), real);
}

Vector3f Rotate::applyTransformation(Vector3f val, int type){
	return quat._transformVector(val);
}

Scale::Scale(float x, float y, float z){
	mat = Matrix4f();
	mat(0, 0) = x;
	mat(1, 1) = y;
	mat(2, 2) = z;
	mat(3, 3) = 1;
}

Vector3f Scale::applyTransformation(Vector3f val, int type){
	Vector4f temp;
	temp(0) = val(0);
	temp(1) = val(1);
	temp(2) = val(2);
	temp(3) = type;
	temp = mat*temp;
	val(0) = temp(0);
	val(1) = temp(1);
	val(2) = temp(2);
	return val;
}