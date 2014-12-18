#pragma once
#include <Windows.h>

#include "GLee.h"
#include "GL/glut.h"
#include "MatrixTransform.h"
#include "BuildingConstructor.h"
#include "Turtle3D.h"
#include "Quad.h"
#include "SOIL.h"

class City {
	public:
	City(MatrixTransform*);
	void init();
	MatrixTransform* getRoot();
	~City();
	
	protected:
	MatrixTransform* root;
	GLuint texture[6];
};

