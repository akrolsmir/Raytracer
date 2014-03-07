class Sample {
public:
	float x, y;

	Sample(float x, float y);
};

class Sampler {
public:
	Sampler(int width, int height);

	Sample* generateSample();
	bool hasNext();
private:
	int width, height;
	float nextX, nextY;
};