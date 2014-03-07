#include "Eigen\Dense"

using namespace Eigen;
using namespace std;

typedef Vector3f Vector;
typedef Vector3f Color;
typedef Vector3f Point;

class Ray {
public:
	Point pos;
	Vector dir;
};

class Scene {
public:
	Point eyePos;
	Point UL, UR, LL, LR;
	int width, height;

	int main() {
		cout << "HELLO WORLD" << endl;
		cin.ignore();
	}
};