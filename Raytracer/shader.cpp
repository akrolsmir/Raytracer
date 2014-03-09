#include "shader.h"

BRDF::BRDF(Color ka, Color kd, Color ks, float sp) :
	kd(kd), ks(ks), ka(ka), sp(sp){/*nothing*/}