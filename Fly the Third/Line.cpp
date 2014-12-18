#include "Line.h"

Line::Line(double _width, double _height, Vector3d _color){
	width = _width;
	height = _height;
	color = _color;
}

void Line::render(){
	glColor3d(color[0], color[1], color[2]);
	glLineWidth(width);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0, height, 0);
	glEnd();
}

Bs Line::update(){
	return bs;
}