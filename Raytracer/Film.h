#include <iostream>
#include <string>
#include <math.h>
#include <windows.h>

#include "FreeImage\FreeImage.h"
#include "color.h"

using namespace std;

class Film {
public:
	int width, height;

	Film(int width, int height);

	void writeImage(string filename);
	void storeSample(float x, float y, Color color);

private:
	//Color* samples;
	float *red, *green, *blue;
	int* count;
	//BYTE* frame; //Stored in BGR, not RGB, to play nice with FreeImage
};