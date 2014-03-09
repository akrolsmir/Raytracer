#ifndef BASIC_H
#define BASIC_H

#include "Eigen\Dense"

using namespace Eigen;
using namespace std;

typedef Vector3f Vector;
typedef Vector3f Color;
typedef Vector3f Point;

class Local{
public:
	Local() : pos(Point()), normal(Vector3f()){};
	Local(Point p, Vector3f norm) :
		pos(p), normal(norm.normalized()) {/*nothing*/
	};
	Point pos;
	Vector3f normal;
};

class Ray{
public:
	Ray(Point p, Vector3f direction, float tMin, float tMax) :
		pos(p), dir(direction), t_min(tMin), t_max(tMax){/*nothing*/
	};

	Point* calculatePosition(float t){
		if (t <= t_min + .001 || t > t_max){
			return NULL;
		}
		Vector3f result = pos + t*dir;
		return new Point(result);
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