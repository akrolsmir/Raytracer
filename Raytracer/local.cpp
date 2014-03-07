#include "local.h"

Local::Local(Point* p, Vector3f* norm) : 
	pos(*p), normal(norm->normalized()) {/*nothing*/}

Point* Local::getPoint(){
	return new Point(pos);
}

Vector3f* Local::getNormal(){
	return new Vector3f(normal(0), normal(1), normal(2));
}