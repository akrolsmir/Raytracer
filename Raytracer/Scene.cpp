#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>

#include "Film.h"
#include "primitive.h"
#include "shape.h"
#include "light.h"

#include "basic.h"
#include "AABB.h"

#define pairwise(a, b) Color(a(0) * b(0), a(1) * b(1), a(2) * b(2));

#define PARSE_ERROR(line, msg, err_line, err) *err_line = line; *err = msg; return false;

using namespace std;

int width, height, max_depth;
int max_verts, max_vert_norms = 0;
float aa = 1;
float aa_step = 1;
string output_name = "output.png";
Camera camera = Camera();
Point UL, UR, LR, LL;
Color ka_scene;
vector<Light*> lights;
bool dof = false;
float depth_dist;

////this is the ugliest shit
//typedef struct {
//	Vector3f pt;
//	Vector3f normal;
//} VertexNormal_t;

int frames = 1;
vector<Vector3f> accelerations, velocities;
vector<GeometricPrimitive*> accels, velos;

vector<vector<Transformation*>> transform_stack;
vector<vector<Transformation*>> inv_transform_stack;
vector<Transformation*> curr_transform;
vector<Transformation*> curr_inv_transform;
vector<Vector3f> verts;
vector<Vector3f> norms;

AggregatePrimitive primitives;
vector<AggregatePrimitive*> curr_obj;
vector<Primitive*> objs;
vector<GeometricPrimitive*> geo_primitives;
AABB* aabb;


// Generates a ray from camera, through the screen coordinates x, y
Ray generateRay(float x, float y) {
	float u = x / width, v = y / height;
	Point p = (1 - u) * ((1 - v) * LL + v * UL) + u * ((1 - v) * LR + v * UR);
	if (!dof){
		return Ray(camera.lookFrom, p - camera.lookFrom, 1, INFINITY);
	}
	else {
		Vector3f start;
		start(0) = -.05 + camera.lookFrom(0) + rand() / (RAND_MAX / .1);
		start(1) = -.05 + camera.lookFrom(1) + rand() / (RAND_MAX / .1);
		start(2) = camera.lookFrom(2);
		return Ray(start, p - start, 1, INFINITY);
	}
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
	return Ray(local.pos, direction);
}

// Returns false if total internal reflection.
// Otherwise puts refraction ray in t and returns true
bool refract(Vector3f d, Vector3f n, float nt, Vector3f* t) {
	float k = 1 - (1 - d.dot(n) * d.dot(n)) / (nt * nt);
	if (k < 0) return false;
	*t = (d - n * d.dot(n)) / nt - n * sqrt(k);
	return true;
}

float *beta = new float, *gamma = new float;
FIBITMAP *negZimage, *posZimage, *negYimage, *posYimage, *negXimage, *posXimage;
vector<Parallelogram> envSides;
vector<FIBITMAP*> envImages;

Color environmentMap(Ray ray) {
	for (int i = 0; i < envSides.size(); i++){
		if (envSides.at(i).intersect(ray, beta, gamma)) {
			RGBQUAD rgbquad;
			FIBITMAP* image = envImages.at(i);
			FreeImage_GetPixelColor(image, (int)(FreeImage_GetWidth(image) * *beta), (int)(FreeImage_GetWidth(image) * *gamma), &rgbquad);
			return Color(rgbquad.rgbRed / 255.0, rgbquad.rgbGreen / 255.0, rgbquad.rgbBlue / 255.0);
		}
	}
	return ka_scene;
}

float* t_hit = new float;
Intersection* in = new Intersection();

Color traceRay(Ray ray, int depth) {
	// Return black if depth exceeds threshold
	if (depth > max_depth) {
		return Color(0, 0, 0);
	}

	// Return black if no intersection
	if (!aabb->intersect(ray, t_hit, in)){
		return environmentMap(ray);
	}

	Color result = ka_scene;

	BRDF brdf = in->primitive->getBRDF();
	BRDF amb = BRDF(brdf.ka, Color(0, 0, 0), Color(0, 0, 0), 0);
	for (Light* light : lights) {
		Ray lightRay = light->generateRay(in->local.pos);
		if (!aabb->intersect(lightRay)){
			/*
			if (in->local.normal.dot(ray.dir) < 0){
				in->local.normal = -in->local.normal;
			}
			*/
			result += shade(in->local, brdf, *light);
		}
		else{
			result += shade(in->local, amb, *light);
		}
		// TODO respond accordingly if refractive object hit
	}

	// Recursively computes refractions
	if (brdf.n) {
		Ray r = reflectRay(in->local, ray);
		Vector3f d = ray.dir.normalized();
		Vector3f n = in->local.normal;
		Vector3f t;
		float c;
		if (d.dot(n) < 0) {
			// Going into refractive surface
			refract(d, n, brdf.n, &t);
			c = -d.dot(n);
		}
		else {
			// Coming out of refractive surface
			if (refract(d, -n, 1 / brdf.n, &t))
				c = t.dot(n);
			else // Total internal reflection
				return result + traceRay(r, depth + 1);
		}
		float R0 = (brdf.n - 1) * (brdf.n - 1) / (brdf.n + 1) * (brdf.n + 1);
		float R = R0 + (1 - R0) * (1 - c) * (1 - c) * (1 - c) * (1 - c) * (1 - c);
		result += R * traceRay(r, depth + 1) + (1 - R) * traceRay(Ray(in->local.pos, t), depth + 1); //TODO consider min
	}

	// Recursively computes reflections
	else if (brdf.kr.norm()) {
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
				ss >> aa;
				aa_step = 1 / aa;
			}
			else if (buf == "output"){
				ss >> output_name;
			}
			else if (buf == "camera"){
				float x, y, z;
				ss >> x >> y >> z;
				if (!dof){
					UL = Point(-1, 1, -1);
					UR = Point(1, 1, -1);
					LR = Point(1, -1, -1);
					LL = Point(-1, -1, -1);
				}
				else {
					UL = Point(-1, 1, -depth_dist);
					UR = Point(1, 1, -depth_dist);
					LR = Point(1, -1, -depth_dist);
					LL = Point(-1, -1, -depth_dist);
				}
				camera.lookFrom = Point(x, y, z);
				for (size_t i = 0; i < transform_stack.size(); i++){
					for (size_t j = 0; j < transform_stack[i].size(); j++){
						camera.lookFrom = transform_stack[i][j]->applyTransformation(camera.lookFrom, 1);
						UL = transform_stack[i][j]->applyTransformation(UL, 1);
						UR = transform_stack[i][j]->applyTransformation(UR, 1);
						LR = transform_stack[i][j]->applyTransformation(LR, 1);
						LL = transform_stack[i][j]->applyTransformation(LL, 1);
					}
				}
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
					for (int i = 0; i < transform_stack.size(); i++){
						curr_obj.back()->addTransform(transform_stack[i], inv_transform_stack[i]);
					}
				}
				else {
					curr_obj.pop_back();
				}
			}
			else if (buf == "sphere"){
				float x, y, z, r;
				ss >> x >> y >> z >> r;
				if (curr_obj.size() == 0){
					GeometricPrimitive* sphere = new GeometricPrimitive(new Sphere(Point(x, y, z), r), BRDF());
					objs.push_back(sphere);
					geo_primitives.push_back(sphere);
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
				geo_primitives.push_back(tri);
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
				curr_inv_transform.insert(curr_inv_transform.begin(), new Translate(-x, -y, -z));
			}
			else if (buf == "rotate"){
				double x, y, z, r;
				ss >> x >> y >> z >> r;
				curr_transform.push_back(new Rotate(Vector3f(x,y,z),r));
				curr_inv_transform.insert(curr_inv_transform.begin(), new Rotate(Vector3f(x, y, z), -r));
			}
			else if (buf == "scale"){
				float x, y, z;
				ss >> x >> y >> z;
				curr_transform.push_back(new Scale(x, y, z));
				curr_inv_transform.insert(curr_inv_transform.begin(), new Scale(1 / x, 1 / y, 1 / z));
			}
			else if (buf == "pushTransform"){
				transform_stack.push_back(curr_transform);
				inv_transform_stack.insert(inv_transform_stack.begin(),curr_inv_transform);
				curr_transform.clear();
				curr_inv_transform.erase(curr_inv_transform.begin());

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
				if (curr_obj.size() == 0){
					objs.back()->setkr(Color(r, g, b));
				}
				else {
					curr_obj.back()->setkr(Color(r, g, b));
				}
			}
			else if (buf == "refraction"){
				float s;
				ss >> s;
				if (curr_obj.size() == 0){
					objs.back()->setn(s);
				}
				else {
					curr_obj.back()->setn(s);
				}
			}
			else if (buf == "envmap"){
				string filename;
				float x1, y1, z1, x2, y2, z2, x3, y3, z3;
				ss >> filename;
				ss >> x1 >> y1 >> z1;
				ss >> x2 >> y2 >> z2;
				ss >> x3 >> y3 >> z3;
				FIBITMAP* image = FreeImage_Load(FreeImage_GetFIFFromFilename(filename.c_str()), filename.c_str());
				Parallelogram side = Parallelogram(Point(x1, y1, z1), Point(x2, y2, z2), Point(x3, y3, z3));
				envImages.push_back(image);
				envSides.push_back(side);
			}
			else if (buf == "dof"){
				ss >> depth_dist;
				dof = true;
			}
			else if (buf == "frames") {
				ss >> frames;
			}
			else if (buf == "velocity") {
				float dx, dy, dz;
				ss >> dx >> dy >> dz;
				velocities.push_back(Vector3f(dx, dy, dz));
				velos.push_back((GeometricPrimitive*)objs.back());
			}
			else if (buf == "acceleration") {
				float dx, dy, dz;
				ss >> dx >> dy >> dz;
				accelerations.push_back(Vector3f(dx, dy, dz));
				accels.push_back((GeometricPrimitive*)objs.back());
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

int main(int argc, char* argv[]) {
	string in;
	if (argc == 2){
		in = argv[1];
	}
	else {
		cout << "Location of input file: ";
		cin >> in;
	}
	ifstream input(in);
	clock_t start = clock();
	std::cout << "Starting clock..." << endl;

	std::cout << "Parsing file..." << endl;
	string error;
	int err_line;
	if (!parse_file(&input, &error, &err_line)){
		cout << "Error: Line " << err_line << ": " << error << endl;
		exit(1);
	}

	std::cout << (clock() - start) / (double)CLOCKS_PER_SEC << "s: " << "Parsing complete" << endl;
	std::cout << "Initializing scene..." << endl;

	// Begin main loop

	Film film = Film(width, height);

	aabb = new AABB(geo_primitives);

	primitives = AggregatePrimitive(objs);

	size_t size = width*height*aa*aa;
	size_t perc = size / 100;
	size_t num_completed = 0;

	std::cout << "Starting render..." << endl;
	for (int i = 0; i < frames; i++) {
		float nextX = 0.5, nextY = 0.5;
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
			if ((++num_completed) % perc == 0){
				cout << "Progress: " << num_completed / perc << "% -- " << (clock() - start) / (double)CLOCKS_PER_SEC << "s\n";
			}
		}

		for (int j = 0; j < accelerations.size(); j++) {
			accels.at(j)->shape->setCenter(accels.at(j)->shape->getCenter() + accelerations.at(j) * i);
		}
		for (int j = 0; j < velocities.size(); j++) {
			velos.at(j)->shape->setCenter(velos.at(j)->shape->getCenter() + velocities.at(j));
		}
	}

	film.writeImage(output_name);
	string num;
	stringstream ss_output(output_name);
	getline(ss_output, num, '-');
	getline(ss_output, num, '.');
	ofstream note("Notes/notes-" + num);
	note << "Input file: " << in;
	note << endl << "Time ran: " << (clock() - start) / (double)CLOCKS_PER_SEC << "s" << endl;
	cout << (clock() - start) / (double)CLOCKS_PER_SEC << "s" << endl;
	cout << "Enter to exit." << endl;
	cin.ignore();
}