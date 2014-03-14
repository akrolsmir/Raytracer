#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>

#include "Film.h"
#include "primitive.h"
#include "shape.h"
#include "light.h"

#include "basic.h"

#define pairwise(a, b) Color(a(0) * b(0), a(1) * b(1), a(2) * b(2));

#define PARSE_ERROR(line, msg, err_line, err) *err_line = line; *err = msg; return false;

using namespace std;


int width, height, max_depth;
int max_verts, max_vert_norms = 0;
float aa_step = 1;
string output_name = "output.png";
Camera camera = Camera();
Point UL, UR, LR, LL;
Color ka_scene;
vector<Light*> lights;

//this is the ugliest shit
typedef struct {
	Vector3f pt;
	Vector3f normal;
} VertexNormal_t;

vector<vector<Transformation*>> transform_stack;
vector<vector<Transformation*>> inv_transform_stack;
vector<Transformation*> curr_transform;
vector<Transformation*> curr_inv_transform;
vector<Vector3f> verts;
vector<Vector3f> norms;

AggregatePrimitive primitives;
vector<AggregatePrimitive*> curr_obj;
vector<Primitive*> objs;

// Generates a ray from camera, through the screen coordinates x, y
Ray generateRay(float x, float y) {
	float u = x / width, v = y / height;
	Point p = (1 - u) * ((1 - v) * LL + v * UL) + u * ((1 - v) * LR + v * UR);
	return Ray(camera.lookFrom, p - camera.lookFrom, 1, INFINITY);
}

Color shade(Local local, BRDF brdf, Light& light) {
	// Ambient term
	Color result = brdf.ka;

	// Diffuse term
	Vector3f direction = light.getDirection(local.pos);
	float dot = direction.dot(local.normal);
	result += max(0, dot) * pairwise(brdf.kd, light.color);

	// Specular term
	Vector3f reflected = (2 * direction.dot(local.normal) * local.normal) - direction;
	dot = reflected.dot((camera.lookFrom - local.pos).normalized());
	result += pow(max(0, dot), brdf.sp) * pairwise(brdf.ks, light.color);

	return result;
}

Ray reflectRay(Local local, Ray ray) {
	Vector3f direction = ray.dir - 2 * ray.dir.dot(local.normal) * local.normal;
	return Ray(local.pos, direction, 0.0001, INFINITY);
}

float* t_hit = new float;
Intersection* in = new Intersection();

Color traceRay(Ray ray, int depth) {
	// Return black if depth exceeds threshold
	if (depth > 5) {
		return Color(0, 0, 0);
	}

	// Return black if no intersection
	if (!primitives.intersect(ray, t_hit, in)){
		return ka_scene;
	}

	Color result = ka_scene;

	BRDF brdf = in->primitive->getBRDF();
	BRDF amb = BRDF(brdf.ka, Color(0,0,0), Color(0,0,0), 0);

	for (Light* light : lights) {
		Ray lightRay = light->generateRay(in->local.pos);
		if (!primitives.intersect(lightRay)){
			result += shade(in->local, brdf, *light);
		}
		else{
			result += shade(in->local, amb, *light);
		}
	}
	if (brdf.kr.norm() > 0) {
		Ray r = reflectRay(in->local, ray);
		Color reflectColor = traceRay(r, depth + 1);
		result += pairwise(brdf.kr, reflectColor);
	}
	return result;
}

/**
 * Parses the input file. Based on Professor Ravi Ramamoorthi's specifications
 * at http://inst.eecs.berkeley.edu/~cs184/fa12/assignments/hw5.pdf
 */
bool parse_file(ifstream* file, string* error, int* err_loc){
	string line;
	string buf;
	size_t num_lines = 0;
	size_t curr_line = 0; //error tracking
	while (getline(*file, line)){
		if (line.length() > 0){
			curr_line++;
			if (line[0] == '#'){
				/* comment, dont eval */
				continue;
			}
			stringstream ss(line);
			ss >> buf;
			if (num_lines++ == 0){
				if (buf != "size"){
					PARSE_ERROR(curr_line, "First line of input must specify image size", err_loc, error);
				}
				ss >> height >> width;
			}
			else if (buf == "maxdepth"){
				ss >> max_depth;
			}
			else if (buf == "aliasing"){
				ss >> aa_step;
				aa_step = 1 / aa_step;
			}
			else if (buf == "output"){
				ss >> output_name;
			}
			else if (buf == "camera"){
				float x, y, z;
				ss >> x >> y >> z;
				camera.lookFrom = Point(x, y, z);
				ss >> x >> y >> z;
				camera.lookAt = Point(x, y, z);
				ss >> x >> y >> z;
				camera.up = Point(x, y, z);
				ss >> camera.fov;
			}
			else if (buf == "g"){
				if (ss >> buf){
					AggregatePrimitive* new_obj = new AggregatePrimitive();
					if (curr_obj.size() == 0){
						objs.push_back(new_obj);
					}
					else {
						curr_obj.back()->addPrimitive(new_obj);
					}
					curr_obj.push_back(new_obj);
				}
				else {
					curr_obj.pop_back();
				}
				for (int i = 0; i < transform_stack.size(); i++){
					curr_obj.back()->addTransform(transform_stack[i], inv_transform_stack[i]);
				}
			}
			else if (buf == "sphere"){
				float x, y, z, r;
				ss >> x >> y >> z >> r;
				if (curr_obj.size() == 0){
					objs.push_back(new GeometricPrimitive(new Sphere(Point(x, y, z), r), BRDF()));
					for (int i = 0; i < transform_stack.size(); i++){
						objs.back()->addTransform(transform_stack[i], inv_transform_stack[i]);
					}
				}
				else {
					GeometricPrimitive* new_p = new GeometricPrimitive(new Sphere(Point(x, y, z), r), BRDF());
					curr_obj.back()->addPrimitive(new_p);
					for (int i = 0; i < transform_stack.size(); i++){
						new_p->addTransform(transform_stack[i], inv_transform_stack[i]);
					}
				}
			}
			else if (buf == "maxverts"){
				ss >> max_verts;
			}
			else if (buf == "maxvertnorms"){
				ss >> max_vert_norms;
			}
			else if (buf == "v"){
				float v1, v2, v3;
				ss >> v1 >> v2 >> v3;
				verts.push_back(Vector3f(v1, v2, v3));
			}
			else if (buf == "vt"){
				/*ignore*/
				continue;
			}
			else if (buf == "vn"){
				float v1, v2, v3;
				ss >> v1 >> v2 >> v3;
				norms.push_back(Vector3f(v1, v2, v3));
			}
			else if (buf == "f"){
				size_t vs[3];
				size_t ns[3];
				bool isNorm = false;
				for (int i = 0; i < 3; i++){
					int count = 0;
					string v;
					string rest;
					ss >> rest;
					stringstream ss_rest(rest);
					while (getline(ss_rest, v, '/') && count++ < 3){
						if (count == 1){
							vs[i] = atoi(v.c_str());
						}
						if (count == 3){
							isNorm = true;
							ns[i] = atoi(v.c_str());
						}
					}
				}
				GeometricPrimitive* tri;
				if (isNorm){
					tri = new GeometricPrimitive(new NormalTriangle(verts[vs[0]-1], verts[vs[1]-1], verts[vs[2]-1], norms[ns[0]-1], norms[ns[1]-1], norms[ns[2]-1]), BRDF());
				}
				else {
					tri = new GeometricPrimitive(new Triangle(verts[vs[0] - 1], verts[vs[1] - 1], verts[vs[2] - 1]), BRDF());
				}
				if (curr_obj.size() == 0){
					objs.push_back(tri);
					for (int i = 0; i < transform_stack.size(); i++){
						objs.back()->addTransform(transform_stack[i], inv_transform_stack[i]);
					}
				}
				else {
					curr_obj.back()->addPrimitive(tri);
					for (int i = 0; i < transform_stack.size(); i++){
						tri->addTransform(transform_stack[i], inv_transform_stack[i]);
					}
				}
			}
			else if (buf == "translate"){
				float x, y, z;
				ss >> x >> y >> z;
				curr_transform.push_back(new Translate(x, y, z));
				curr_inv_transform.push_back(new Translate(-x, -y, -z));
			}
			else if (buf == "rotate"){
				double x, y, z, r;
				ss >> x >> y >> z >> r;
				curr_transform.push_back(new Rotate(Vector3f(x,y,z),r));
				curr_inv_transform.push_back(new Rotate(Vector3f(x,y,z),-r));
			}
			else if (buf == "scale"){
				float x, y, z;
				ss >> x >> y >> z;
				curr_transform.push_back(new Scale(x, y, z));
				curr_inv_transform.push_back(new Scale(1/x, 1/y, 1/z));
			}
			else if (buf == "pushTransform"){
				transform_stack.push_back(curr_transform);
				inv_transform_stack.push_back(curr_inv_transform);
				curr_transform.clear();
				curr_inv_transform.clear();

			}
			else if (buf == "popTransform"){
				transform_stack.pop_back();
				inv_transform_stack.pop_back();
			}
			else if (buf == "directional"){
				float x, y, z, r, g, b;
				ss >> x >> y >> z >> r >> g >> b;
				lights.push_back(new DirectionLight(x, y, z, r, g, b));
			}
			else if (buf == "point"){
				float x, y, z, r, g, b;
				ss >> x >> y >> z >> r >> g >> b;
				lights.push_back(new PointLight(x, y, z, r, g, b));
			}
			else if (buf == "ambient"){
				float r, g, b;
				ss >> r >> g >> b;
				ka_scene = Point(r, g, b);
			}
			else if (buf == "diffuse"){
				float r, g, b;
				ss >> r >> g >> b;
				if (curr_obj.size() == 0){
					objs.back()->setkd(Color(r, g, b));
				}
				else {
					curr_obj.back()->setkd(Color(r, g, b));
				}
			}
			else if (buf == "specular"){
				float r, g, b;
				ss >> r >> g >> b;
				if (curr_obj.size() == 0){
					objs.back()->setks(Color(r, g, b));
				}
				else {
					curr_obj.back()->setks(Color(r, g, b));
				}
			}
			else if (buf == "shininess"){
				float s;
				ss >> s;
				if (curr_obj.size() == 0){
					objs.back()->setsp(s);
				}
				else {
					curr_obj.back()->setsp(s);
				}
			}
			else if (buf == "emission"){
				float r, g, b;
				ss >> r >> g >> b;
				if (curr_obj.size() == 0){
					objs.back()->setka(Color(r, g, b));
				}
				else {
					curr_obj.back()->setka(Color(r, g, b));
				}
			}
			else if (buf == "reflectance"){
				float r, g, b;
				ss >> r >> g >> b;
				objs.back()->getBRDFPointer()->kr = Color(r, g, b);
			}
			else {
				/*disallow everything else*/
				PARSE_ERROR(curr_line, "Unexpected token", err_loc, error);
			}

			/** Check that the line has no more valid characters to read */
			if ((ss >> buf) && buf[0] != '#'){
				PARSE_ERROR(curr_line, "Expected new line", err_loc, error);
			}
		}
		
	}
	return true;
}

int main() {
	clock_t start = clock();
	std::cout << "Starting clock..." << endl;

	std::cout << (clock() - start) / (double)CLOCKS_PER_SEC << "s: " << "Parsing file..." << endl;
	ifstream input("input.test");
	string error;
	int err_line;
	if (!parse_file(&input, &error, &err_line)){
		cout << "Error: Line " << err_line << ": " << error << endl;
		exit(1);
	}

	UL = Point(-1, 1, -3);
	UR = Point(1, 1, -3);
	LR = Point(1, -1, -3);
	LL = Point(-1, -1, -3);

	// Begin main loop
	float nextX = 0.5, nextY = 0.5;

	Film film = Film(width, height);

	//objs.push_back(new GeometricPrimitive(new Triangle(Point(0.0, 0.0, 0.0), Point(0.0, 0.5, 0.0), Point(0.5, 0.0, 0.0)), 
	//		BRDF(Color(0.1, 0.1, 0.1), Color(0.3, 0.3, 0.0), Color(0.8, 0.8, 0.8), 0)));

	primitives = AggregatePrimitive(objs);

	std::cout << "Starting render..." << endl;
	while (nextY <= height) {
		Ray ray = generateRay(nextX, nextY);
		Color color = traceRay(ray, 0);
		film.storeSample(nextX, nextY, color);

		// Calculate next sample location
		nextX += aa_step;
		if (nextX >= width) {
			nextY += aa_step;
			nextX = 0.5;
		}
	}

	film.writeImage(output_name);
	cout << (clock() - start) / (double)CLOCKS_PER_SEC << "s" << endl;
	cout << "Enter to exit." << endl;
	cin.ignore();
}