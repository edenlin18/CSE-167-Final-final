#pragma once

#include <time.h>
#include "MatrixTransform.h"
#include "Building.h"

class BuildingConstructor {
	public:
	BuildingConstructor(GLuint[6], Matrix4d, GLdouble, GLdouble, int, int);
	void construct();
	void update();
	MatrixTransform* getRoot();
	~BuildingConstructor();

	protected:
	MatrixTransform* root;
	GLuint* texture;
	GLdouble maxH;
	GLdouble minH;
	int layerCount;
	int choice;
	
};

