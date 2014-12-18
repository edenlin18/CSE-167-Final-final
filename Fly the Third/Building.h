#pragma once
#include "MatrixTransform.h"
#include "Cube.h"

const double BUILDING_SIZE = 1.0;
const double HALF_BUILDING_SIZE = BUILDING_SIZE / 2.0;
const double TALL_BUILDING_MAX_HEIGHT = 10.0;
const double TALL_BUILDING_MIN_HEIGHT = 5.0;
const double MEDIUM_BUILDING_MAX_HEIGHT = 5.0;
const double MEDIUM_BUILDING_MIN_HEIGHT = 3.0;

class Building {
	public:
	// Buildings(GLdouble length, GLdouble width, GLdouble height, Matrix4);
	Building(GLuint buildingChoice, GLuint roofChoice, double r, double g, double b, GLuint* texture, GLdouble, GLdouble, GLdouble, Matrix4d);
	MatrixTransform* getRoot();
	~Building();

	protected:
	MatrixTransform* root;
	GLdouble length;
	GLdouble width;
	GLdouble height;
};

