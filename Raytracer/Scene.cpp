#include <iostream>
#include <time.h>
#include <vector>

#include "Film.h"
#include "color.h"
#include "point.h"
#include "ray.h"
#include "shape.h"
#include "light.h"
#include "local.h"

#define pairwise(a, b) Color(a.getR() * b.getR(), a.getG() * b.getG(), a.getB() * b.getB())

using namespace std;

int width = 500, height = 500;

Point camera = Point(0, 0, 5);
Point UL = Point(-1, 1, 0), UR = Point(1, 1, 0), LR = Point(1, -1, 0), LL = Point(-1, -1, 0);

vector<Light*> lights;

Sphere sphere = Sphere(new Point(0, 0, -2), 0.4);

struct BRDF {
	Color ka, kd, ks, kr;
	float sp;
};

// Generates a ray from camera, through the screen coordinates x, y
Ray* generateRay(float x, float y) {
	float u = x / width, v = y / height;
	Point p = (1 - u) * ((1 - v) * LL + v * UL) + u * ((1 - v) * LR + v * UR);
	return new Ray(&camera, &(Vector3f)(p - camera), 1, INFINITY);
}

Color* shade(Local* local, BRDF* brdf, Light* light) {
	Color* result = new Color();

	// Ambient term
	*result += brdf->ka;

	// Diffuse term
	Vector3f direction = *light->getDirection(local->getPoint());
	float dot = direction.dot(*local->getNormal());
	dot = max(0, dot);
	*result += dot * pairwise(brdf->kd, (*light->color));

	// Specular term
	Vector3f reflected = (2 * direction.dot(*local->getNormal()) * *local->getNormal()) - direction;
	dot = pow(max(0.0, reflected.dot((camera - *local->getPoint()).normalized())), brdf->sp);
	Color specular = Color(dot*pairwise(brdf->ks, (*light->color)));
	//cout << "Specular term:\n" << specular << "\n\n";
	*result += specular;

	//Austin's version
	//Vector3f reflect = 2 * direction.dot(*local->getNormal()) * (*local->getNormal()) - direction;
	//Vector3f viewer = camera - *local->getPoint();
	//float dot_s = reflect.dot(viewer.normalized());
	//dot_s = max(0, dot_s);
	//*result += pow(dot_s, brdf->sp) * pairwise(brdf->ks, (*light->color));	

	return result;
}

Color* traceRay(Ray* ray, int depth) {
	// Return black if depth exceeds threshold
	if (depth > 4) {
		return new Color(0, 0, 0);
	}

	float* t_hit = new float;
	Point* p = new Point();
	Vector3f* n = new Vector3f();
	Local* local = new Local(p, n); //TODO why doesn't local have a default initializer? This memory leaks.

	// Return black if no intersection
	if (!sphere.intersect(ray, t_hit, local)) { //TODO replace with AggregatePrimitive or acceleration structure
		return new Color(0, 0, 0);
	}

	// TODO removeemporary test BRDF
	BRDF brdf;
	brdf.ka = Color(0.1, 0.1, 0.0);
	brdf.kd = Color(1.0, 1.0, 0.0);
	brdf.ks = Color(0.8, 0.8, 0.8);
	brdf.sp = 16;

	Color* result = new Color(0, 0, 0);
	for (Light* light : lights) {
		*result += *shade(local, &brdf, light);
	}
	//TODO handle mirror and shading
	return result;
}

int main() {
	clock_t start = clock();
	float nextX = 0.5, nextY = 0.5;
	Color black = Color(0, 0, 0), red = Color(1, 0, 0);

	Film film = Film(width, height);

	lights.push_back(new PointLight(200, 200, 200, 0.6, 0.6, 0.6));

	while (nextY <= height) {
		film.storeSample(nextX, nextY, *traceRay(generateRay(nextX, nextY), 0));

		// Calculate next sample location
		float step = 1;
		nextX += step;
		if (nextX >= width) {
			nextY += step;
			nextX = 0.5;
		}
	}
	
	cout << (clock() - start) / (double)CLOCKS_PER_SEC << "s" << endl;
	film.writeImage("output.png");
	cout << (clock() - start) / (double)CLOCKS_PER_SEC << "s" << endl;
	cout << "Enter to exit." << endl;
	//cin.ignore();
}