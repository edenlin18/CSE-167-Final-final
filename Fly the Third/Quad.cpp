#include "Quad.h"

Quad::Quad() {
}

void Quad::render() {
	glBegin(GL_QUADS);
	glColor3f(0.3, 0.3, 0.3);		// This makes the cube green; the parameters are for red, green and blue.

	// Draw bottom side:
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(-1.0, 0.0, -1.0);
	glVertex3f(-1.0, 0.0, 1.0);
	glVertex3f(1.0, 0.0, 1.0);
	glVertex3f(1.0, 0.0, -1.0);
	
	glEnd();
}

Bs Quad::update() {
	return Bs();
}

Quad::~Quad() {
}
