#include <iostream>
#include <time.h>

#include "Film.h"
#include "color.h"
#include "Sampler.h"

using namespace std;

int main() {
	clock_t start = clock();

	int width = 500, height = 500;

	Sampler sampler = Sampler(width, height);
	Color black = Color(0, 0, 0);

	Film film = Film(width, height);

	while (sampler.hasNext()) {
		Sample* sample = sampler.generateSample();
		film.storeSample(sample->x, sample->y, black);
		free(sample);
	}
	
	cout << (clock() - start) / (double)CLOCKS_PER_SEC << "s" << endl;
	film.writeImage("output.png");
	cout << (clock() - start) / (double)CLOCKS_PER_SEC << "s" << endl;
	//cin.ignore();
}