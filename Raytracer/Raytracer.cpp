
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include <time.h>
#include <math.h>
#include <string>

#include "light.h"
#include "FreeImage\FreeImage.h"

#define PI 3.14159265  // Should be used from mathlib
inline float sqr(float x) { return x*x; }

using namespace std;

//****************************************************
// Some Classes
//****************************************************
class Viewport;

class Viewport {
public:
	int w = 400, h = 400; // width and height
};


//****************************************************
// Global Variables
//****************************************************
Viewport	viewport;
// frames is stored in BGR, not RGB, to accomodate FreeImage
BYTE** frames = NULL;
int maxFrames = 1;

Vec3 *ka = new Vec3();
Vec3 *kd = new Vec3();
Vec3 *ks = new Vec3();
float sp;

Vec3* viewer = new Vec3(0, 0, 1);

vector<Light*> lights;
vector<PointLight*> pointLights;

bool isAnisotropic = false;
float nu, nv;

Vec3* silhouette = NULL;
Vec3* cw = NULL;
Vec3* cc = NULL;

size_t gradient = 0;

//****************************************************
// Assigns a pixel in a frame the specified color
//****************************************************
void setPixel(int frame, int x, int y, float r, float g, float b){
	r = min(1, max(0, r));
	g = min(1, max(0, g));
	b = min(1, max(0, b));

	frames[frame][3 * (x + y * viewport.w)] = (BYTE)(b * 255);
	frames[frame][3 * (x + y * viewport.w) + 1] = (BYTE)(g * 255);
	frames[frame][3 * (x + y * viewport.w) + 2] = (BYTE)(r * 255);
}

void setPixel(int frame, int x, int y, Vec3 rgb) {
	setPixel(frame, x, y, rgb.elems[0], rgb.elems[1], rgb.elems[2]);
}


//****************************************************
// Shader functions
//****************************************************
Vec3* ambient(){
	return ka;
}

Vec3* diffuse(Vec3* direction, Vec3* surface_normal, Vec3* intensity, Vec3* diffuse_reflectance){
	float dot = max(0.0, dotProd(direction, surface_normal));
	Vec3* result = point_mult(intensity, diffuse_reflectance);
	result->scalar_mult(dot);
	return result;
}

Vec3* anisotropic_diffuse(Vec3* direction, Vec3* surface_normal, Vec3* viewer,
	Vec3* intensity, Vec3* diffuse_reflectance, Vec3* specular_reflectance){
	float constant = max(0.0, 28 / (23 * PI)*(1 - pow(1 - dotProd(surface_normal, direction) / 2, 5)) * (1 - pow(1 - dotProd(surface_normal, viewer) / 2, 5)));
	Vec3* result = new Vec3(1, 1, 1);
	result->sub(specular_reflectance);
	result->point_mult(diffuse_reflectance);
	result->scalar_mult(constant);
	result->point_mult(intensity);
	return result;
}


Vec3* reflect(Vec3* direction, Vec3* surface_normal){
	float scaled_dot = 2 * dotProd(direction, surface_normal);
	Vec3* result = scalar_mult(scaled_dot, surface_normal);
	result->sub(direction);
	return result;
}

Vec3* specular(Vec3* reflected, Vec3* viewer, Vec3* intensity, float power, Vec3* specular_reflectance){
	float dot = pow(max(0.0, dotProd(reflected, viewer)), power);
	Vec3* result = point_mult(intensity, specular_reflectance);
	result->scalar_mult(dot);
	return result;
}

Vec3* anisotropic_specular(Vec3* direction, Vec3* surface_normal, Vec3* u, Vec3* v,
	Vec3* viewer, Vec3* intensity, Vec3* specular_reflectance, float nu, float nv){
	/*
	* Ignores Fresnel fraction and some terms for simplicity
	*/
	Vec3* half_vector = add(direction, viewer);
	half_vector->normalize();
	float hu = dotProd(half_vector, u);
	float hv = dotProd(half_vector, v);
	float hn = dotProd(half_vector, surface_normal);
	float exponent = (nu*pow(hu, 2) + nv*pow(hv, 2)) / (1 - pow(hn, 2));
	Vec3* result = point_mult(intensity, specular_reflectance);
	result->scalar_mult(sqrt((nu + 1)*(nv + 1)) * pow(max(0.0, hn), exponent) / (8 * PI));
	delete half_vector;
	return result;
}

Vec3* cool_to_warm(Vec3* direction, Vec3* surface_normal, Vec3* cw, Vec3* cc){
	float kw = (1 + dotProd(direction, surface_normal)) / 2;
	Vec3* result = scalar_mult(kw, cw);
	Vec3* inter = scalar_mult(1 - kw, cc);
	result->add(inter);
	delete inter;
	return result;
}

void discretize(Vec3* color, size_t gradient){
	for (size_t i = 0; i < 3; i++){
		color->elems[i] = ceil(color->elems[i] * gradient) / gradient;
	}
}

//****************************************************
// Simple init function
//****************************************************
void initScene(){
}

void renderFrames(){
	// Make the BYTE array, factor of 3 because it's RBG.
	frames = new BYTE*[maxFrames];
	for (int i = 0; i < maxFrames; i++) {
		frames[i] = new BYTE[3 * viewport.w * viewport.h];
		for (int j = 0; j < 3 * viewport.w * viewport.h; j++){
			frames[i][j] = 0;
		}
	}

	// Fill in frames
	for (int frame = 0; frame < maxFrames; frame++){
		cout << "Rendering frame " << frame << "...\n";
	}
}


//****************************************************
// Write the image to a file
//****************************************************
void writeImage(string filename) {
	renderFrames();

	// Code from http://stackoverflow.com/a/6938633/1222351
	// Convert to FreeImage format & save to file
	FIBITMAP* image = FreeImage_ConvertFromRawBits(frames[maxFrames - 1], viewport.w, viewport.h,
		3 * viewport.w, 24, 0x0000FF, 0xFF0000, 0x00FF00, false);
	FreeImage_Save(FreeImage_GetFIFFromFilename(filename.c_str()), image, filename.c_str(), 0);

	// Free resources
	FreeImage_Unload(image);
}

//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
//int main(int argc, char *argv[]) {
//	int i = 1;
//	string opt, filename;
//	float x, y, z, r, g, b;
//
//	while (i < argc){
//		opt = argv[i];
//
//		if (opt == "-ka"){			// set ambient color coefficients
//			r = atof(argv[i + 1]), g = atof(argv[i + 2]), b = atof(argv[i + 3]);
//			*ka = Vec3(r, g, b);
//			i += 4;
//		}
//		else if (opt == "-kd"){		// set diffuse color coefficients
//			r = atof(argv[i + 1]), g = atof(argv[i + 2]), b = atof(argv[i + 3]);
//			*kd = Vec3(r, g, b);
//			i += 4;
//		}
//		else if (opt == "-ks"){		// set specular color coefficients
//			r = atof(argv[i + 1]), g = atof(argv[i + 2]), b = atof(argv[i + 3]);
//			*ks = Vec3(r, g, b);
//			i += 4;
//		}
//		else if (opt == "-sp"){		// set specular power coefficient
//			sp = atof(argv[i + 1]);
//			i += 2;
//		}
//		else if (opt == "-pl"){		// add a point light
//			x = atof(argv[i + 1]), y = atof(argv[i + 2]), z = atof(argv[i + 3]);
//			r = atof(argv[i + 4]), g = atof(argv[i + 5]), b = atof(argv[i + 6]);
//			PointLight* pl = new PointLight(x, y, z, r, g, b);
//			lights.push_back(pl);
//			pointLights.push_back(pl);
//			i += 7;
//		}
//		else if (opt == "-dl"){		// add a directional light
//			x = atof(argv[i + 1]), y = atof(argv[i + 2]), z = atof(argv[i + 3]);
//			r = atof(argv[i + 4]), g = atof(argv[i + 5]), b = atof(argv[i + 6]);
//			DirectionLight* pl = new DirectionLight(x, y, z, r, g, b);
//			lights.push_back(pl);
//			i += 7;
//		}
//		else if (opt == "-wi"){		// write image to file
//			filename = argv[i + 1];
//			i += 2;
//		}
//		else if (opt == "-wh"){		// set initial width and height
//			viewport.w = atoi(argv[i + 1]);
//			viewport.h = atoi(argv[i + 2]);
//			i += 3;
//		}
//		else if (opt == "-an"){		// animate
//			maxFrames = atoi(argv[i + 1]);
//			i += 2;
//		}
//		else if (opt == "-as"){		// anisotropic brdf
//			isAnisotropic = true;
//			nu = atof(argv[i + 1]);
//			nv = atof(argv[i + 2]);
//			i += 3;
//		}
//		else if (opt == "-sil"){	// silhouette
//			silhouette = new Vec3(atof(argv[i + 1]), atof(argv[i + 2]), atof(argv[i + 3]));
//			i += 4;
//		}
//		else if (opt == "-cw"){		// warm-to-cool shading
//			cw = new Vec3(atof(argv[i + 1]), atof(argv[i + 2]), atof(argv[i + 3]));
//			cc = new Vec3(atof(argv[i + 4]), atof(argv[i + 5]), atof(argv[i + 6]));
//			i += 7;
//		}
//		else if (opt == "-t"){      // toon shading
//			gradient = atoi(argv[i + 1]);
//			i += 2;
//		}
//		else {						// bad cmd line argument
//			cout << "ERROR: Received \"" << opt << "\" as flag.\n";
//			i++;
//		}
//	}
//	if (filename.size() > 0)
//		writeImage(filename);
//	return 0;
//}








