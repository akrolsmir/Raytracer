#include "primitive.h"
#include <iostream>
#define UNIMPLEMENTED(str) std::cout << "Unimplemented method " << str << std::endl; exit(0);

bool Primitive::intersect(Ray ray, float* t_hit, Intersection* in){
	UNIMPLEMENTED("Intersect");
}

bool Primitive::intersect(Ray ray){
	UNIMPLEMENTED("Intersect");
}

void Primitive::addTransform(vector<Transformation*> trans, vector<Transformation*> invTrans){
	UNIMPLEMENTED("addTransform");
}

BRDF Primitive::getBRDF(){
	UNIMPLEMENTED("getBRDF");
}

BRDF* Primitive::getBRDFPointer(){
	UNIMPLEMENTED("getBRDFPointer");
}

void Primitive::setkd(Color kd){
	UNIMPLEMENTED("setkd");
}
void Primitive::setks(Color ks){
	UNIMPLEMENTED("setks");
}
void Primitive::setka(Color ka){
	UNIMPLEMENTED("setka");
}
void Primitive::setsp(float sp){
	UNIMPLEMENTED("setsp");
}


GeometricPrimitive::GeometricPrimitive(Shape* shape, BRDF brdf) :
	shape(shape), brdf(brdf){/*nothing*/}

Vector3f GeometricPrimitive::getCenter(){
	return shape->getCenter();
}

Vector3f GeometricPrimitive::getMinBB(){
	return shape->getMinBB();
}
Vector3f GeometricPrimitive::getMaxBB(){
	return shape->getMaxBB();
}

bool GeometricPrimitive::intersect(Ray ray, float* t_hit, Intersection* in){
	Ray oray = ray;
	Local oLocal = Local();
	for (int i = worldToObj.size() - 1; i >= 0; i--){
		oray.dir = worldToObj[i]->applyTransformation(oray.dir, 0);
		oray.pos = worldToObj[i]->applyTransformation(oray.pos, 1);
	}
	Shape* test = shape;
	if (!shape->intersect(oray, t_hit, &oLocal)){
		return false;
	}
	in->primitive = this;
	for (int i = objToWorld.size() - 1; i >= 0; i--){
		oLocal.pos = objToWorld[i]->applyTransformation(oLocal.pos, 1);
		oLocal.normal = objToWorld[i]->applyTransformation(oLocal.normal, 0).normalized();
	}
	in->local = oLocal;
	return true;
}
bool GeometricPrimitive::intersect(Ray ray){
	Ray oray = ray;
	for (int i = worldToObj.size() - 1; i >= 0; i--){
		oray.dir = worldToObj[i]->applyTransformation(oray.dir, 0);
		oray.pos = worldToObj[i]->applyTransformation(oray.pos, 1);
	}
	return shape->intersect(oray);
}

void GeometricPrimitive::addTransform(vector<Transformation*> trans, vector<Transformation*> invTrans){
	objToWorld.insert(objToWorld.end(), trans.begin(), trans.end());
	worldToObj.insert(worldToObj.end(), invTrans.begin(), invTrans.end());
}

BRDF GeometricPrimitive::getBRDF(){	
	return brdf;
}

BRDF* GeometricPrimitive::getBRDFPointer(){
	return &brdf;
}

void GeometricPrimitive::setkd(Color kd){
	brdf.kd = kd;
}
void GeometricPrimitive::setks(Color ks){
	brdf.ks = ks;
}
void GeometricPrimitive::setka(Color ka){
	brdf.ka = ka;
}
void GeometricPrimitive::setsp(float sp){
	brdf.sp = sp;
}

AggregatePrimitive::AggregatePrimitive() :
	primitives(std::vector<Primitive*>()){/*nothing*/}

AggregatePrimitive::AggregatePrimitive(std::vector<Primitive*> lst) :
	primitives(lst){/*nothing*/}

bool AggregatePrimitive::intersect(Ray ray, float* t_hit, Intersection* in){
	Intersection nextIn = Intersection();
	float nextT = INFINITY;
	float currT = INFINITY;
	bool hit = false;
	for (size_t i = 0; i < primitives.size(); i++){
		if (primitives[i]->intersect(ray, &nextT, &nextIn)){
			hit = true;
			if (nextT < currT){
				currT = nextT;
				*t_hit = currT;
				*in = nextIn;
			}
		}
	}
	return hit;
}
bool AggregatePrimitive::intersect(Ray ray){
	for (size_t i = 0; i < primitives.size(); i++){
		if (primitives[i]->intersect(ray)){
			return true;
		}
	}
	return false;
}

void AggregatePrimitive::addPrimitive(Primitive* p){
	primitives.push_back(p);
}

void AggregatePrimitive::addTransform(vector<Transformation*> trans, vector<Transformation*> invTrans){
	for (size_t i = 0; i < primitives.size(); i++){
		primitives[i]->addTransform(trans, invTrans);
	}
}


BRDF AggregatePrimitive::getBRDF(){
	UNIMPLEMENTED("getBRDF");
}

BRDF* AggregatePrimitive::getBRDFPointer(){
	UNIMPLEMENTED("getBRDFPointer");
}

void AggregatePrimitive::setkd(Color kd){
	for (size_t i = 0; i < primitives.size(); i++){
		primitives[i]->setkd(kd);
	}
}
void AggregatePrimitive::setks(Color ks){
	for (size_t i = 0; i < primitives.size(); i++){
		primitives[i]->setks(ks);
	}
}
void AggregatePrimitive::setka(Color ka){
	for (size_t i = 0; i < primitives.size(); i++){
		primitives[i]->setka(ka);
	}
}
void AggregatePrimitive::setsp(float sp){
	for (size_t i = 0; i < primitives.size(); i++){
		primitives[i]->setsp(sp);
	}
}

Intersection::Intersection() :
local(Local()), primitive(NULL){/*nothing*/
}

Intersection::Intersection(Local loc, Primitive* primitive) :
local(loc), primitive(primitive){/*nothing*/
}
