#include <iostream>
#include <time.h>

#include "Film.h"
#include "color.h"
#include "Sampler.h"

using namespace std;

int main() {
	clock_t start = clock();

	int width = 300, height = 300;

	Sampler sampler = Sampler(width, height);
	Color black = Color(0, 0, 0);

	Film film = Film(width, height);

	while (sampler.hasNext()) {
		Sample* sample = sampler.generateSample();
		film.storeSample(sample->x, sample->y, black);
		free(sample);
	}
	
	film.writeImage("output.png");
	cout << (clock() - start) / (double)CLOCKS_PER_SEC << "s" << endl;
	cin.ignore();
}