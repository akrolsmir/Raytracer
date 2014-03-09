#include "intersection.h"

/**
 * Honestly this should probably be somewhere else and not
 * taking up another file but lazy.
 */

Intersection::Intersection() :
	local(Local()), primitive(NULL){/*nothing*/}

Intersection::Intersection(Local loc, Primitive* primitive) :
	local(loc), primitive(primitive){/*nothing*/}