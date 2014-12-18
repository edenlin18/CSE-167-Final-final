#pragma once

#include <time.h>
#include <math.h>

#include <Windows.h>
#include <GLee.h>
#include <GL/glut.h>

#include "MatrixTransform.h"

#include "City.h"

#include "Turtle3D.h"

#include <iostream>

using namespace std;

/*
struct Building{
	float RGB[3];
	Matrix4 matrix;
	const static int MAX = 100;
};
*/

class Scene{
private:

	const bool DEBUG = false;

	City* city;

	MatrixTransform* root;

	Turtle3D* turtle;
	MatrixTransform* mt;

	/*
	GLuint objectList;
	Building object[Building::MAX];
	bool made;
	*/

public:

	Scene();
	~Scene();

	void initialize();
	void update();
	void draw(Matrix4d);
	
	void drawDefault();

	void debug(const string& comment);

	/*
	void building(const GLfloat& x, const GLfloat& z, Building&);
	*/
};

