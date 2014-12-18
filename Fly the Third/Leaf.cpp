#include "Leaf.h"

Leaf::Leaf(double _width, double _height, Vector3d _color, Material *_m){
	width = _width;
	height = _height;
	color = _color;
	m = _m;
}

void Leaf::render(){
	if (m != NULL){
		m->apply();
	}
	else
		glColor3d(color[0], color[1], color[2]);
	glBegin(GL_TRIANGLES);
	glVertex3f(0, 0, 0);
	glVertex3f(width/2, 0, height/2);
	glVertex3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(-width/2, 0, -height/2);
	glVertex3f(0, 1, 0);
	glEnd();
}

Bs Leaf::update(){

	return bs;
}