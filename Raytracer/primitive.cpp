#include "primitive.h"
#include <iostream>
#define UNIMPLEMENTED(str) std::cout << "Unimplemented method " << str << std::endl; exit(0);

bool Primitive::intersect(Ray* ray, float* t_hit, Intersection* in){
	UNIMPLEMENTED("Intersect");
}

bool Primitive::intersect(Ray* ray){
	UNIMPLEMENTED("Intersect");
}

BRDF Primitive::getBRDF(){
	UNIMPLEMENTED("getBRDF");
}

GeometricPrimitive::GeometricPrimitive(Shape* shape, BRDF brdf) :
	shape(shape), brdf(brdf){/*nothing*/}

bool GeometricPrimitive::intersect(Ray* ray, float* t_hit, Intersection* in){
	//TODO transformations
	/*
	Point oPos = Point(worldToObj * *ray->getPos());
	Vector3f oDir = worldToObj * *ray->getPos();
	Ray oray = Ray(&oPos, &oDir, ray->getTMin(), ray->getTMax());
	Local oLocal = Local();
	if (!shape->intersect(&oray, t_hit, &oLocal)){
		return false;
	}
	in->primitive = this;
	in->local.pos = (Point) (objToWorld*oLocal.pos);
	in->local.normal = (Point)(objToWorld*oLocal.normal);
	in->local.normal.normalize();
	*/
	Local oLocal = Local();
	if (!shape->intersect(ray, t_hit, &oLocal)){
		return false;
	}
	in->primitive = this;
	in->local = oLocal;
	return true;


}
bool GeometricPrimitive::intersect(Ray* ray){
	//TODO transformations
	/*
	Point oPos = Point(worldToObj * *ray->getPos());
	Vector3f oDir = worldToObj * *ray->getPos();
	Ray oray = Ray(&oPos, &oDir, ray->getTMin(), ray->getTMax());
	return shape->intersect(&oray);
	*/
	return shape->intersect(ray);
}

BRDF GeometricPrimitive::getBRDF(){
	return brdf;
}

AggregatePrimitive::AggregatePrimitive() :
	primitives(std::vector<Primitive*>()){/*nothing*/}

AggregatePrimitive::AggregatePrimitive(std::vector<Primitive*> lst) :
	primitives(lst){/*nothing*/}

bool AggregatePrimitive::intersect(Ray* ray, float* t_hit, Intersection* in){
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
bool AggregatePrimitive::intersect(Ray* ray){
	for (size_t i = 0; i < primitives.size(); i++){
		if (primitives[i]->intersect(ray)){
			return true;
		}
	}
	return false;
}

BRDF AggregatePrimitive::getBRDF(){
	UNIMPLEMENTED("getBRDF");
}

Intersection::Intersection() :
local(Local()), primitive(NULL){/*nothing*/
}

Intersection::Intersection(Local loc, Primitive* primitive) :
local(loc), primitive(primitive){/*nothing*/
}
