#include "AABB.h"
#include "cmath"

AABB::AABB(){/*nothing*/}

float compare_vectors(Vector3f a, Vector3f b){
	Vector3f temp = a - b;
	return temp(0) + temp(1) + temp(2);
}

AABB* make_closest(vector<GeometricPrimitive*> &lst){
	double distance = INFINITY;
	double temp = 0;
	size_t k, l;
	GeometricPrimitive* closest[2];
	if (lst.size() == 0){
		return NULL;
	}
	if (lst.size() == 1){
		GeometricPrimitive* node = lst[0];
		lst.pop_back();
		vector<GeometricPrimitive*> temp_v;
		temp_v.push_back(node);
		return new AABB(temp_v);
	}
	for (vector<GeometricPrimitive*>::iterator it = lst.begin(); it != lst.end(); it++){
		for (vector<GeometricPrimitive*>::iterator ij = it+1; ij != lst.end(); ij++){
			temp = ((*it)->getCenter() - (*ij)->getCenter()).norm();
			if (temp < distance){
				distance = temp;
				k = min(it - lst.begin(), ij - lst.begin());
				l = max(it - lst.begin(), ij - lst.begin());
				closest[0] = *it;
				closest[1] = *ij;
			}
		}
	}
	vector<GeometricPrimitive*> temp_vect = vector<GeometricPrimitive*>(lst.begin(), lst.begin() + k);
	temp_vect.insert(temp_vect.end(), lst.begin() + k + 1, lst.begin() + l);
	temp_vect.insert(temp_vect.end(), lst.begin() + l + 1, lst.end());
	lst = temp_vect;
	vector<GeometricPrimitive*> temp_v;
	temp_v.push_back(closest[0]);
	temp_v.push_back(closest[1]);
	return new AABB(temp_v);
}

AABB* make_closest(vector<AABB*>& lst){
	double distance = INFINITY;
	double temp = 0;
	size_t k, l;
	AABB* closest[2];
	if (lst.size() == 1){
		AABB* new_node = new AABB(lst[0], NULL);
		lst.pop_back();
		return new_node;
	}
	for (size_t i = 0; i < lst.size(); i++){
		for (size_t j = i + 1; j < lst.size(); j++){
			temp = ((lst[j]->maximum - lst[j]->minimum) / 2 - (lst[i]->maximum - lst[i]->minimum) / 2).norm();
			if (temp < distance){
				distance = temp;
				k = min(i, j);
				l = max(i, j);
				closest[0] = lst[i];
				closest[1] = lst[j];
			}
		}
	}
	vector<AABB*> temp_vect = vector<AABB*>(lst.begin(), lst.begin() + k);
	temp_vect.insert(temp_vect.end(), lst.begin() + k + 1, lst.begin() + l);
	temp_vect.insert(temp_vect.end(), lst.begin() + l + 1, lst.end());
	lst = temp_vect;
	return new AABB(closest[0], closest[1]);
}

AABB::AABB(vector<GeometricPrimitive*> lst){
	if (lst.size() == 1){
		left = new AABB_Leaf(lst[0]);
		left->parent = this;
		right = NULL;
		minimum = left->minimum;
		maximum = left->maximum;
	}
	else if (lst.size() == 2){
		left = new AABB_Leaf(lst[0]);
		right = new AABB_Leaf(lst[1]);
		left->parent = this;
		right->parent = this;
		if (compare_vectors(left->minimum, right->minimum) < 0){
			minimum = left->minimum;
		}
		else {
			minimum = right->minimum;
		}
		if (compare_vectors(left->maximum, right->maximum) < 0){
			maximum = right->maximum;
		}
		else {
			minimum = left->minimum;
		}
	}
	else{
		vector<AABB*> nodes;
		while (lst.size() > 0){
			nodes.push_back(make_closest(lst));
		}
		while (nodes.size() > 1){
			vector<AABB*> temp_nodes = vector<AABB*>();
			while (nodes.size() > 0){
				temp_nodes.push_back(make_closest(nodes));
			}
			nodes = temp_nodes;
		}
		this->left = nodes[0];
		this->right = NULL;
		this->minimum = this->left->minimum;
		this->maximum = this->left->maximum;
	}
}

AABB::AABB(AABB* a, AABB* b){
	this->left = a;
	this->right = b;
	if (a == NULL || b == NULL){
		if (a == NULL){
			minimum = right->minimum;
			maximum = right->maximum;
		}
		else if (b == NULL){
			minimum = left->minimum;
			maximum = left->maximum;
		}
	}
	else {
		if (compare_vectors(left->minimum, right->minimum) < 0){
			minimum = left->minimum;
		}
		else {
			minimum = right->minimum;
		}
		if (compare_vectors(left->maximum, right->maximum) < 0){
			maximum = right->maximum;
		}
		else {
			minimum = left->minimum;
		}
	}
}

bool intersection(Ray ray, Vector3f minimum, Vector3f maximum){
	float t_near = -INFINITY;
	float t_far = INFINITY;
	for (int i = 0; i < 3; i++){
		if (ray.dir(i) == 0){
			if (ray.pos(i) < minimum(i) || ray.pos(i) > minimum(i)){
				return false;
			}
		}
		else {
			float t1 = (minimum(i) - ray.pos(i)) / ray.dir(i);
			float t2 = (maximum(i) - ray.pos(i)) / ray.dir(i);
			if (t1 > t2){
				float temp = t1;
				t1 = t2;
				t2 = temp;
			}
			if (t1 > t_near){
				t_near = t1;
			}
			if (t2 < t_far){
				t_far = t2;
			}
			if (t_near > t_far){
				return false;
			}
			if (!ray.inBounds(t_near) && !ray.inBounds(t_far)){
				return false;
			}
		}
	}
	return true;
}

/**
 * Intersections computed with the slab method from
 * http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter3.htm
 */
bool AABB::intersect(Ray ray, float* t_hit, Intersection* in){
	if (intersection(ray, minimum, maximum)){
		if (left != NULL && left->intersect(ray, t_hit, in)){
			return true;
		}
		else if (right != NULL && right->intersect(ray, t_hit, in)){
			return true;
		}
	}
	return false;
}
bool AABB::intersect(Ray ray){
	if (intersection(ray, minimum, maximum)){
		if (left != NULL && left->intersect(ray)){
			return true;
		}
		else  if (right != NULL && right->intersect(ray)){
			return true;
		}
	}
	return false;
}
 
AABB_Leaf::AABB_Leaf(GeometricPrimitive* p){
	this->p = p;
	minimum = p->getMinBB();
	maximum = p->getMaxBB();
	Vector3f box[8];
	cout << minimum << endl;
	float deltaX = this->maximum(0) - this->minimum(0);
	float deltaY = this->maximum(1) - this->minimum(1);
	float deltaZ = this->maximum(2) - this->minimum(2);
	box[0] = this->minimum;
	box[1] = this->minimum + Vector3f(deltaX, 0, 0);
	box[2] = this->minimum + Vector3f(0, deltaY, 0);
	box[3] = this->minimum + Vector3f(0, 0, deltaZ);
	box[4] = this->minimum + Vector3f(deltaX, deltaY, 0);
	box[5] = this->minimum + Vector3f(0, deltaY, deltaZ);
	box[6] = this->minimum + Vector3f(deltaX, 0, deltaZ);
	box[7] = this->maximum;

	for (int i = p->objToWorld.size() - 1; i >= 0; i--){
		for (int j = 0;  j < 8; j++){
			box[j] = p->objToWorld[i]->applyTransformation(box[j], 1);
		}
	}
	float minX = min(box[0](0), min(box[1](0), min(box[2](0), min(box[3](0), min(box[4](0), min(box[5](0), min(box[6](0), box[7](0))))))));
	float minY = min(box[0](1), min(box[1](1), min(box[2](1), min(box[3](1), min(box[4](1), min(box[5](1), min(box[6](1), box[7](1))))))));
	float minZ = min(box[0](2), min(box[1](2), min(box[2](2), min(box[3](2), min(box[4](2), min(box[5](2), min(box[6](2), box[7](2))))))));
	float maxX = max(box[0](0), max(box[1](0), max(box[2](0), max(box[3](0), max(box[4](0), max(box[5](0), max(box[6](0), box[7](0))))))));
	float maxY = max(box[0](1), max(box[1](1), max(box[2](1), max(box[3](1), max(box[4](1), max(box[5](1), max(box[6](1), box[7](1))))))));
	float maxZ = max(box[0](2), max(box[1](2), max(box[2](2), max(box[3](2), max(box[4](2), max(box[5](2), max(box[6](2), box[7](2))))))));
	this->minimum = Vector3f(minX, minY, minZ);
	this->maximum = Vector3f(maxX, maxY, maxZ);
	this->left = NULL;
	this->right = NULL;
}

bool AABB_Leaf::intersect(Ray ray, float* t_hit, Intersection* in){
	return p->intersect(ray, t_hit, in);
}
bool AABB_Leaf::intersect(Ray ray){
	return p->intersect(ray);
}