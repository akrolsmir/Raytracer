#include <iostream>
#include <time.h>
#include <vector>

#include "Film.h"
#include "primitive.h"
#include "shape.h"
#include "light.h"

#include "basic.h"

#define pairwise(a, b) Color(a(0) * b(0), a(1) * b(1), a(2) * b(2));

using namespace std;

int width, height;
Point camera, UL, UR, LR, LL;
vector<Light*> lights;

AggregatePrimitive primitives;

// Generates a ray from camera, through the screen coordinates x, y
Ray* generateRay(float x, float y) {
	float u = x / width, v = y / height;
	Point p = (1 - u) * ((1 - v) * LL + v * UL) + u * ((1 - v) * LR + v * UR);
	return new Ray(camera, p - camera, 1, INFINITY);
}

void shade(Local* local, BRDF* brdf, Light* light, Color* result) {
	// Ambient term
	*result += brdf->ka;

	// Diffuse term
	Vector3f direction = *light->getDirection(&local->pos);
	float dot = direction.dot(local->normal);
	*result += max(0, dot) * pairwise(brdf->kd, (*light->color));

	// Specular term
	Vector3f reflected = (2 * direction.dot(local->normal) * local->normal) - direction;
	dot = reflected.dot((camera - local->pos).normalized());
	*result += pow(max(0, dot), brdf->sp) * pairwise(brdf->ks, (*light->color));
}

float* t_hit = new float;
Local* local = new Local();

Color* traceRay(Ray ray, int depth) {
	// Return black if depth exceeds threshold
	if (depth > 4) {
		return new Color(0, 0, 0);
	}

	Intersection* in = new Intersection();
	Color* result = new Color(0, 0, 0);
	// Return black if no intersection
	if (!primitives.intersect(ray, t_hit, in)){
		return new Color(0, 0, 0);
	}

	BRDF brdf = in->primitive->getBRDF();
	BRDF amb = BRDF(brdf.ka, Color(0,0,0), Color(0,0,0), 0);

	for (Light* light : lights) {
		Ray* lightRay = light->generateRay(&(in->local.pos));
		if (!primitives.intersect(*lightRay)){
			shade(&(in->local), &brdf, light, result);
		}
		else{
			shade(&(in->local), &amb, light, result);
		}
		delete lightRay;
	}
	//TODO handle mirror and shading
	return result;
}

int main() {
	clock_t start = clock();
	cout << "Starting clock..." << endl;

	// Hardcoded test values
	width = 2000, height = 2000;

	camera = Point(0, 0, 10);
	UL = Point(-1, 1, 1);
	UR = Point(1, 1, 1);
	LR = Point(1, -1, 1);
	LL = Point(-1, -1, 1);

	// Begin main loop
	float nextX = 0.5, nextY = 0.5;

	Film film = Film(width, height);

	vector<Primitive*> objs;
	objs.push_back(new GeometricPrimitive(new Sphere(Point(0.0, 0.0, -5.0), 0.4),
	BRDF(Color(0.1, 0.1, 0.0), Color(1.0, 1.0, 0.0), Color(0.8, 0.8, 0.8), 16)));
	objs.push_back(new GeometricPrimitive(new Sphere(Point(0.5, 0.5, -5.0), 0.4),
	BRDF(Color(0.1, 0.1, 0.0), Color(1.0, 1.0, 0.0), Color(0.8, 0.8, 0.8), 16)));
	objs.push_back(new GeometricPrimitive(new Sphere(Point(-0.5, -0.5, -10.0), 1.0),
	BRDF(Color(0.1, 0.1, 0.0), Color(1.0, 1.0, 0.0), Color(0.8, 0.8, 0.8), 16)));
	objs.push_back(new GeometricPrimitive(new Triangle(Point(-1.0, 0.0, -10.0), Point(0.0, 1.8, -11.0), Point(1.5, 0.0, -12.0)), 
			BRDF(Color(0.1, 0.1, 0.1), Color(0.0, 0.0, 1.0), Color(0.0, 0.0, 0.0), 0)));

	primitives = AggregatePrimitive(objs);

	lights.push_back(new PointLight(3, 3, 3, 0.6, 0.6, 0.6));
	lights.push_back(new PointLight(200, 200, 200, 0.6, 0.6, 0.6));
	lights.push_back(new DirectionLight(0, 1, -1, 0, 0.4, 0.4));

	Ray* ray;
	Color* color;

	cout << "Starting render..." << endl;

	while (nextY <= height) {
		ray = generateRay(nextX, nextY);
		color = traceRay(*ray, 0);
		film.storeSample(nextX, nextY, *color);

		// Calculate next sample location
		float step = 1;
		nextX += step;
		if (nextX >= width) {
			nextY += step;
			nextX = 0.5;
		}
		delete ray;
		delete color;
	}
	film.writeImage("output3.png");
	cout << (clock() - start) / (double)CLOCKS_PER_SEC << "s" << endl;
	cout << "Enter to exit." << endl;
	cin.ignore();
}