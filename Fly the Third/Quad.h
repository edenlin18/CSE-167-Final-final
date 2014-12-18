#pragma once
#include "Geode.h"

class Quad : public Geode {
	public:
	Quad();
	Bs update();
	~Quad();

	protected:
	void render();
};

