#ifndef _CUBE_H_
#define _CUBE_H_
#include <windows.h>

#include <iostream>
#include "GLee.h"
#include "GL/glut.h"
#include "Geode.h"
#include "Vector3d.h"
#include "Const.h"
#include "Material.h"

class Cube : public Geode{

public:
	Cube(GLuint* texture, const int& buildingChoice, const int& roofChoice, const GLdouble& xScaling, const GLdouble& yScaling, const GLdouble& zScaling, double s, Vector3d c, draw::mode m, Material * ma = NULL);
	Bs update();
private:
	void render();
	void drawBox(GLfloat);
	Material * material;
	int buildingChoice;
	int roofChoice;
	GLuint* texture;
	GLdouble xScaling;
	GLdouble yScaling;
	GLdouble zScaling;
	double size;
	Vector3d color; 
	draw::mode mode;
};

#endif