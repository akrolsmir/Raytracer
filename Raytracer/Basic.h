#ifndef BASIC_H
#define BASIC_H

#include "Eigen\Dense"

using namespace Eigen;
using namespace std;

typedef Vector3f Vector;
typedef Vector3f Color;
typedef Vector3f Point;

class Camera{
public:
	Point lookFrom;
	Point lookAt;
	Vector3f up;
	float fov;
};

class Local{
public:
	Local(){};
	Local(Point p, Vector3f norm) :
		pos(p), normal(norm.normalized()) {/*nothing*/
	};
	Point pos;
	Vector3f normal;
};

class Ray{
public:
	Ray(){};
	Ray(Point p, Vector3f direction, float tMin, float tMax) :
		pos(p), dir(direction), t_min(tMin), t_max(tMax){/*nothing*/
	};

	Point calculatePosition(float t){
		if (!inBounds(t)){
			return Point();
		}
		Vector3f result = pos + t*dir;
		return Point(result);
	}

	bool inBounds(float t) {
		return t_min + .0001 < t && t < t_max;
	}

	Point pos;
	Vector3f dir;
	float t_min, t_max;
};

class BRDF{
public:
	Color ka, kd, ks, kr;
	float sp;
	BRDF() :
		kd(Color()), ks(Color()), ka(Color()), sp(0.0){/*nothing*/
	}

	BRDF(Color ka, Color kd, Color ks, float sp) :
		kd(kd), ks(ks), ka(ka), sp(sp){/*nothing*/
	}

};


#endif BASIC_H