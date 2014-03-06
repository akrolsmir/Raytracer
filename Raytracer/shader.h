#ifndef SHADER_H
#define SHADER_H

#include "Vec3.h"

//****************************************************
// Calculate the diffuse component of a point
// Input: The direction of light, the vector normal
//        to the surface, the intensity of light, and
//        the diffuse color constants.
//        Note that direction and surface_normal are
//        normal vectors
// Output: The diffuse component of the perceived light
//****************************************************
Vec3* diffuse(Vec3* direction, Vec3* surface_normal, Vec3* intensity, Vec3* diffuse_reflectance);

//****************************************************
// Calculate the anisotropic diffuse component of a point
// Input: The direction of light, the vector normal
//        to the surface, the viewer vector,
//        the intensity of light, the diffuse constant,
//        and the reflectance constant.
// Output: The diffuse component of the perceived light
//****************************************************
Vec3* anisotropic_diffuse(Vec3* direction, Vec3* surface_normal, Vec3* viewer,
	Vec3* intensity, Vec3* diffuse_reflectance, Vec3* specular_reflectance);

//****************************************************
// Calculate the reflected direction of light
// Input: The direction of light and the vector normal
//        to the surface
// Output: The direction of light reflected from the surface
//****************************************************
Vec3* reflect(Vec3* direction, Vec3* surface_normal);

//****************************************************
// Calculate the specular component of a point
// Input: The reflected direction of light, the viewer
// Output: The specular component of the perceived light
//****************************************************
Vec3* specular(Vec3* reflected, Vec3* viewer, Vec3* intensity, float power, Vec3* specular_reflectance);

//****************************************************
//Calculate the specular component of a point with an
//anisotropic BRDF
//Input: The direction of light, the vector normal to the
//			 surface, unit tangent vectors to the surface,
//       the location, the voiewer,
//       the intensity of light, the specular
//			 color constant, and the shape of the specular
//			 lobe.
//Output: The diffuse component of the perceived light
//****************************************************
Vec3* anisotropic_specular(Vec3* direction, Vec3* surface_normal, Vec3* u, Vec3* v,
	Vec3* viewer, Vec3* intensity, Vec3* specular_reflectance, float nu, float nv);

//****************************************************
//Calculate cool-to-warm shading of a point
//Input: The direction of light, the vector normal to 
//       the surface, the cool color, and the warm
//       color.
//Output: The perceived light
//****************************************************
Vec3* cool_to_warm(Vec3* direction, Vec3* surface_normal, Vec3* cw, Vec3* cc);

//****************************************************
// Discretizes the color. Used with toon shading
// Input: The color and the number of gradients
// Ouput: Modifies the color vector
//****************************************************
void discretize(Vec3* color, size_t gradient);

#endif