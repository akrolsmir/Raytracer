#ifndef SHADER_H
#define SHADER_H

#include "color.h"

class BRDF{
public:
	Color kd;
	Color ks;
	Color ka;
	float sp;

	BRDF(Color ka, Color kd, Color ks, float sp);
};

#endif