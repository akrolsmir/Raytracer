#include "light.h"
#include <math.h>

Vec3* add(Vec3* x, Vec3* y){
	Vec3 *result = new Vec3();
	for (int i = 0; i < 3; i++){
		result->elems[i] = x->elems[i] + y->elems[i];
	}
	return result;
}

Vec3* sub(Vec3* x, Vec3* y){
	Vec3* result = new Vec3();
	for (int i = 0; i < 3; i++){
		result->elems[i] = x->elems[i] - y->elems[i];
	}
	return result;
}

float dotProd(Vec3* x, Vec3* y){
	float result = 0.0;
	for (int i = 0; i < 3; i++){
		result += (x->elems[i] * y->elems[i]);
	}
	return result;
}

Vec3* crossProd(Vec3* x, Vec3* y){
	return new Vec3(x->elems[1] * y->elems[2] - x->elems[2] * y->elems[1],
		x->elems[2] * y->elems[0] - x->elems[0] * y->elems[2],
		x->elems[0] * y->elems[1] - x->elems[1] * y->elems[0]);
}

Vec3* scalar_mult(float x, Vec3* y){
	Vec3* result = new Vec3();
	for (int i = 0; i < 3; i++){
		result->elems[i] = x * y->elems[i];
	}
	return result;
}

Vec3* point_mult(Vec3* x, Vec3* y){
	Vec3* result = new Vec3();
	for (int i = 0; i < 3; i++){
		result->elems[i] = x->elems[i] * y->elems[i];
	}
	return result;
}

Vec3* cp_normal(Vec3* x){
	Vec3* result = new Vec3();
	float n = x->norm();
	for (int i = 0; i < 3; i++){
		result->elems[i] = x->elems[i] / n;
	}
	return result;
}

Vec3::Vec3(){
	Vec<3>::Vec<3>();
}

Vec3::Vec3(float x, float y, float z){
	elems[0] = x;
	elems[1] = y;
	elems[2] = z;
	size = 3;
}

void Vec3::crossProd(Vec3* x){
	elems[0] = elems[1] * x->elems[2] - elems[2] * x->elems[1];
	elems[1] = elems[2] * x->elems[0] - elems[0] * x->elems[2];
	elems[2] = elems[0] * x->elems[1] - elems[1] * x->elems[0];
}