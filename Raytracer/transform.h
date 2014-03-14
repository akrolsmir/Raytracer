/**
 * Transformations!
 */

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Eigen\Dense"

using namespace Eigen;

class Transformation{
public:
	virtual Vector3f applyTransformation(Vector3f val, int type);
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

class Translate : public Transformation{
public:
	Translate(float tx, float ty, float tz);

	Vector3f applyTransformation(Vector3f val, int type);

private:
	Matrix4f mat;
	Matrix4f norm_mat;
};

class Rotate : public Transformation{
public:
	/**
	 * Angle is in radians
	 */
	Rotate(Vector3f axis, double angle);

	Vector3f applyTransformation(Vector3f val, int type);

private:
	Quaternionf quat;
};

class Scale : public Transformation{
public:
	Scale(float x, float y, float z);

	Vector3f applyTransformation(Vector3f val, int type);

private:
	Matrix4f mat;
};
#endif