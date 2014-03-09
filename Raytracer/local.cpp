#include "local.h"

Local::Local() :
	pos(Point()), normal(Vector3f(0.0, 0.0, 0.0)) {/* nothing */}

Local::Local(Point p, Vector3f norm) : 
	pos(p), normal(norm.normalized()) {/*nothing*/}

//Point Local::getPoint(){
//	return new Point(pos);
//}
//
//Vector3f Local::getNormal(){
//	return new Vector3f(normal(0), normal(1), normal(2));
//}
