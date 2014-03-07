#include "Sampler.h"

Sampler::Sampler(int width, int height) : width(width), height(height) {
	nextX = 0.5;
	nextY = 0.5;
}

Sample* Sampler::generateSample() {
	Sample* sample = new Sample(nextX, nextY);
	if (++nextX >= width) {
		nextY++;
		nextX = 0;
	}
	return sample;
}

bool Sampler::hasNext() {
	return nextY <= height;
}

Sample::Sample(float x, float y) : x(x), y(y){};