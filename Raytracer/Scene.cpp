#include <iostream>
#include <time.h>

#include "Film.h"
#include "color.h"
using namespace std;

int main() {
	clock_t start = clock();
	int width = 1000, height = 1000;
	float nextX = 0.5, nextY = 0.5;
	Color black = Color(0, 0, 0);

	Film film = Film(width, height);

	while (nextY <= height) {
		film.storeSample(nextX, nextY, black);

		// Calculate next sample location
		if (++nextX >= width) {
			nextY++;
			nextX = 0.5;
		}
	}
	
	cout << (clock() - start) / (double)CLOCKS_PER_SEC << "s" << endl;
	film.writeImage("output.png");
	cout << (clock() - start) / (double)CLOCKS_PER_SEC << "s" << endl;
	cin.ignore();
}