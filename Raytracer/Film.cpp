#include "Film.h"

using namespace std;

Film::Film(int width, int height) : width(width), height(height) {
	red = new float[width * height];
	green = new float[width * height];
	blue = new float[width * height];
	count = new int[width * height];
	//Color black = Color(0, 0, 0);
	for (int i = 0; i < width * height; i++) {
		count[i] = 0;
		red[i] = 0;
		green[i] = 0;
		blue[i] = 0;
		//samples[i] = black;
	}
	// TODO consider storing as large Eigen matrix for performance
}


//****************************************************
// Assigns a pixel in the frame the specified color
//****************************************************
void Film::storeSample(float x, float y, Color color){
	int xInt = (int) round(x - 0.5);
	int yInt = (int) round(y - 0.5);
	size_t index = min(width * height - 1, max(0, xInt + yInt * width));
	red[index] += min(1, max(0, color(0)));
	green[index] += min(1, max(0, color(1)));
	blue[index] += min(1, max(0, color(2)));
	count[index]++;
}

//****************************************************
// Write the image to a file
//****************************************************
void Film::writeImage(string filename) {
	BYTE* frame = new BYTE[3 * width * height];

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			//Color color = samples[i + j * width] / count[i + j * width];

			//cout << "i: " << i << " j: " << j << " sample: " << count[i + j * width] << endl;

			//float r = min(1, max(0, color(0)));
			//float g = min(1, max(0, color(1)));
			//float b = min(1, max(0, color(2)));
			frame[3 * (i + j * width)    ] = (BYTE)(blue[i + j * width] / count[i + j * width] *255);
			frame[3 * (i + j * width) + 1] = (BYTE)(green[i + j * width] / count[i + j * width] *255);
			frame[3 * (i + j * width) + 2] = (BYTE)(red[i + j * width] / count[i + j * width] *255);

			//cout << "i: " << i << " j: " << j << " r: " << r << endl;
		}
	}

	// Code from http://stackoverflow.com/a/6938633/1222351
	// Convert to FreeImage format & save to file
	FIBITMAP* image = FreeImage_ConvertFromRawBits(frame, width, height,
		3 * width, 24, 0x0000FF, 0xFF0000, 0x00FF00, false);
	FreeImage_Save(FreeImage_GetFIFFromFilename(filename.c_str()), image, filename.c_str(), 0);

	// Free resources
	FreeImage_Unload(image);
}