#ifndef GRAPHVECTOR_H
#define GRAPHVECTOR_H

/**
* Defines a vector of length I
*/
template<size_t I> class Vec{
public:

	/**
	* Default constructor sets all elements to 0
	*/
	Vec(){
		size = I;
		for (size_t i = 0; i < I; i++){
			elems[i] = 0.0;
		}
	}

	/**
	* Returns true if and only if I am normal.
	*/
	bool isNormal(){
		return norm() == 1.0;
	}

	/**
	* Returns my norm.
	*/
	float norm(){
		float result = 0.0;
		for (size_t i = 0; i < size; i++){
			result += pow(elems[i], 2);
		}
		return sqrt(result);
	}

	/**
	* Normalizes me
	*/
	void normalize(){
		float n = norm();
		for (size_t i = 0; i < size; i++){
			elems[i] = elems[i] / n;
		}
	}


	/**
	* Adds x to me
	*/
	void add(Vec<I>* x){
		for (int i = 0; i < I; i++){
			elems[i] += x->elems[i];
		}
	}

	/**
	* Subtracts x to me
	*/
	void sub(Vec<I>* x){
		for (size_t i = 0; i < I; i++){
			elems[i] -= x->elems[i];
		}
	}

	/**
	* Multiplies x to with my elements
	*/
	void scalar_mult(float x){
		for (size_t i = 0; i < I; i++){
			elems[i] *= x;
		}
	}

	/**
	* Multiplies x's elements with my elements
	*/
	void point_mult(Vec<I>* x){
		for (size_t i = 0; i < I; i++){
			elems[i] *= x->elems[i];
		}
	}

	/**
	* Gets the size of the vector
	*/
	unsigned int getSize() { return size; }

	/**
	* The elements of the vector
	*/
	float elems[I];

protected:
	unsigned int size;
};

/**
* Returns x + y
*/
template<size_t I> Vec<I>* add(Vec<I>* x, Vec<I>* y){
	Vec<I>* result = new Vec<I>();
	for (size_t i = 0; i < I; i++){
		result->elems[i] = x->elems[i] + y->elems[i];
	}
	return *result;
}

/**
* Returns x - y
*/
template<size_t I> Vec<I>* sub(Vec<I>* x, Vec<I>* y){
	Vec<I>* result = new Vec<I>();
	for (size_t i = 0; i < I; i++){
		result->elems[i] = x->elems[i] - y->elems[i];
	}
	return *result;
}

/**
* Returns the dot productof x and y
*/
template<size_t I> float dotProd(Vec<I>* x, Vec<I>* y){
	float result = 0.0;
	for (size_t i = 0; i < I; i++){
		result += (x->elems[i] * y->elems[i]);
	}
	return result;
}

/**
* Returns x * y
*/
template<size_t I> Vec<I>* scalar_mult(float x, Vec<I>* y){
	Vec<I>* result = new Vec<I>();
	for (size_t i = 0; i < I; i++){
		result.elems[i] = x * y->elems[i];
	}
	return *result;
}

/**
* Returns the pointwise multiplication of x and y
*/
template<size_t I> Vec<I>* point_mult(Vec<I>* x, Vec<I>* y){
	Vec<I>* result = new Vec<I>();
	for (int i = 0; i < I; i++){
		result.elems[i] = x->elems[i] * y->elems[i];
	}
	return *result;
}

/**
* Returns a normal vector in the same direction as x
*/
template<size_t I> Vec<I>* cp_normal(Vec<I>* x){
	Vec<I>* result = Vec<I>();
	float n = x->norm();
	for (int i = 0; i < I; i++){
		result->elems[i] = result->elems[i] / n;
	}
	return *result;
}

#endif