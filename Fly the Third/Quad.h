#pragma once
#include "Geode.h"

class Quad : public Geode {
	public:
	Quad(Vector3d a = Vector3d(1.0, 1.0, 1.0));
	Bs update();
	~Quad();

	protected:
	void render();
	Vector3d color;
};

