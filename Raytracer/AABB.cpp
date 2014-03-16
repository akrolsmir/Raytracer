#include "AABB.h"
#include "cmath"
#define UNIMPLEMENTED(str) std::cout << "Unimplemented method " << str << std::endl; exit(0);

AABB_Tree::AABB_Tree(){/*nothing*/}

bool AABB_Tree::intersect(Ray ray, float* t_hit, Intersection* in){
	UNIMPLEMENTED("intersect");
}
bool AABB_Tree::intersect(Ray ray){
	UNIMPLEMENTED("intersect");
}

bool comp_aabb(AABB* a, AABB* b){
	Vector3f diff = (a->minimum + a->maximum) / 2 - (b->minimum + b->maximum)/ 2;
	return diff(0) + diff(1) + diff(2) > 0;
}

bool comp_geo(GeometricPrimitive* a, GeometricPrimitive* b){
	Vector3f diff = a->getCenter() - b->getCenter();
	return diff(0) + diff(1) + diff(2) > 0;
}

Vector3f min_vectors(Vector3f a, Vector3f b){
	return Vector3f(min(a(0), b(0)), min(a(1), b(1)), min(a(2), b(2)));
}

Vector3f max_vectors(Vector3f a, Vector3f b){
	return Vector3f(max(a(0), b(0)), max(a(1), b(1)), max(a(2), b(2)));
}

AABB* make_closest(vector<GeometricPrimitive*> &lst){
	double distance = INFINITY;
	double temp = 0;
	AABB* result;
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
		result = new AABB(temp_v);
		temp_v.pop_back();
		return result;
	}
	/*
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
	*/
	closest[0] = lst.back();
	lst.pop_back();
	closest[1] = lst.back();
	lst.pop_back();
	/*
	vector<GeometricPrimitive*> temp_vect = vector<GeometricPrimitive*>(lst.begin(), lst.begin() + k);
	temp_vect.insert(temp_vect.end(), lst.begin() + k + 1, lst.begin() + l);
	temp_vect.insert(temp_vect.end(), lst.begin() + l + 1, lst.end());
	lst = temp_vect;
	*/
	vector<GeometricPrimitive*> temp_v;
	temp_v.push_back(closest[0]);
	temp_v.push_back(closest[1]);
	result = new AABB(temp_v);
	temp_v.pop_back();
	temp_v.pop_back();
	return result;
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
	/*
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
	*/
	closest[0] = lst.back();
	lst.pop_back();
	closest[1] = lst.back();
	lst.pop_back();
	/*
	vector<AABB*> temp_vect = vector<AABB*>(lst.begin(), lst.begin() + k);
	temp_vect.insert(temp_vect.end(), lst.begin() + k + 1, lst.begin() + l);
	temp_vect.insert(temp_vect.end(), lst.begin() + l + 1, lst.end());
	lst = temp_vect;
	*/
	return new AABB(closest[0], closest[1]);
}

AABB::AABB(vector<GeometricPrimitive*>& lst){
	if (lst.size() == 1){
		left = new AABB_Leaf(lst[0]);
		//left->parent = this;
		right = NULL;
		minimum = left->minimum;
		maximum = left->maximum;
	}
	else if (lst.size() == 2){
		AABB_Leaf* l = new AABB_Leaf(lst[0]);
		this->left = l;
		AABB_Leaf* r = new AABB_Leaf(lst[1]);
		this->right = r;
		this->minimum = min_vectors(l->minimum, r->minimum);
		this->maximum = max_vectors(l->maximum, r->maximum);
	}
	else{
		int len = lst.size();
		sort(lst.begin(), lst.end(), comp_geo);
		vector<AABB*> nodes;
		while (lst.size() > 0){
			nodes.push_back(make_closest(lst));
			nodes.back()->d_from_root = len;
		}
		while (nodes.size() > 1){
			len = nodes.size();
			sort(nodes.begin(), nodes.end(), comp_aabb);
			vector<AABB*> temp_nodes = vector<AABB*>();
			while (nodes.size() > 0){
				temp_nodes.push_back(make_closest(nodes));
				temp_nodes.back()->d_from_root = len;
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
	left = a;
	right = b;
	if (a == NULL || b == NULL){
		if (a == NULL){
			minimum = b->minimum;
			maximum = b->maximum;
		}
		else if (b == NULL){
			minimum = a->minimum;
			maximum = a->maximum;
		}
	}
	else {
		minimum = min_vectors(a->minimum, b->minimum);
		maximum = max_vectors(a->maximum, b->maximum);
	}
}

bool intersection(Ray ray, Vector3f minimum, Vector3f maximum){
	float t_near = -INFINITY;
	float t_far = INFINITY;
	for (int i = 0; i < 3; i++){
		if (ray.dir(i) == 0){
			if (ray.pos(i) < minimum(i) || ray.pos(i) > maximum(i)){
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
		}
	}
	if (t_near > t_far){
		return false;
	}
	/*
	if (!ray.inBounds(t_near) && !ray.inBounds(t_far)){
		return false;
	}
	*/
	return true;
}

int depth = 0;

/**
 * Intersections computed with the slab method from
 * http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter3.htm
 */
bool AABB::intersect(Ray ray, float* t_hit, Intersection* in){
	bool hit = false;
	float left_t = INFINITY;
	float right_t = INFINITY;
	Intersection in_left = Intersection();
	Intersection in_right = Intersection();
	if (intersection(ray, minimum, maximum)){
		depth += 1;
		if (left != NULL && left->intersect(ray, t_hit, &in_left)){
			hit = true;
			left_t = *t_hit;
		}
		if (right != NULL && right->intersect(ray, t_hit, &in_right)){
			hit = true;
			right_t = *t_hit;
		}
		depth -= 1;
	}
	if (hit){
		if (left_t < right_t){
			*t_hit = left_t;
			*in = in_left;
		}
		else{
			*t_hit = right_t;
			*in = in_right;
		}
	}

	return hit;
}
bool AABB::intersect(Ray ray){
	if (intersection(ray, minimum, maximum)){
		if (left != NULL && left->intersect(ray)){
			return true;
		}
		else if (right != NULL && right->intersect(ray)){
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
	Vector3f zeros = Vector3f(0, 0, 0);
	Vector3f deltas = maximum - minimum;
	float deltaX = deltas(0);
	float deltaY = deltas(1);
	float deltaZ = deltas(2);
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