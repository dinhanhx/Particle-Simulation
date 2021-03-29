#pragma once
#include"Particle.h"
#include<tuple>
#include<time.h>
#include<GL/glut.h>
using namespace std;

class WaterParticle
{
private:
	Particle rightHydro;
	Particle leftHydro;
	Particle oxy;
	tuple <float, float, float> position; //x, y, z position
	tuple <float, float, float> velocity; //x, y, z velocity
	int id;
public:
	WaterParticle(tuple <float, float, float> position, int id) {
		this->position = position;
		this->id = id;

		// Assign random velocity for x, y, z
		srand(static_cast <unsigned> (time(0)));
		float LO = 0.000f;
		float HI = 0.005f;
		float x_vec = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
		float y_vec = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
		float z_vec = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
		velocity = make_tuple(x_vec, y_vec, z_vec);

		oxy = Particle(position, make_tuple(242, 240, 198), 0.1f, id);
		rightHydro = Particle(make_tuple(get<0>(position) + 0.15f, get<1>(position), get<2>(position)), make_tuple(31, 81, 255), 0.05f, id);
		leftHydro = Particle(make_tuple(get<0>(position) - 0.15f, get<1>(position), get<2>(position)), make_tuple(31, 81, 255), 0.05f, id);

		oxy.loadCustomBmp("metal.bmp");
	}

	// Get functions
	tuple <float, float, float> getPosition() { return position; }

	// Move H20
	void move(GLfloat lowerBoundary, GLfloat upperBoundary) {
		// bouncing x
		if (get<0>(position) > upperBoundary || get<0>(position) < lowerBoundary) get<0>(velocity) = -1.0f * get<0>(velocity);
		// bouncing y
		if (get<1>(position) > upperBoundary || get<1>(position) < lowerBoundary) get<1>(velocity) = -1.0f * get<1>(velocity);
		// bouncing z
		if (get<2>(position) > upperBoundary || get<2>(position) < lowerBoundary) get<2>(velocity) = -1.0f * get<2>(velocity);

		// move x
		get<0>(position) = get<0>(position) + get<0>(velocity);

		// move y
		get<1>(position) = get<1>(position) + get<1>(velocity);

		// move z
		get<2>(position) = get<2>(position) + get<2>(velocity);

		oxy.setPosition(position);
		rightHydro.setPosition(make_tuple(get<0>(position) + 0.15f, get<1>(position), get<2>(position)));
		leftHydro.setPosition(make_tuple(get<0>(position) - 0.15f, get<1>(position), get<2>(position)));
	}

	void draw() {
		oxy.draw();
		rightHydro.draw();
		leftHydro.draw();
	}
};
