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
	void treeGrow(MatrixTransform* mt);
	~City();
	
	protected:
	MatrixTransform* root;
	MatrixTransform* treeRoot[4];
	GLuint texture[6];
};

