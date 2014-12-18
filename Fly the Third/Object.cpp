#include "Object.h"
Object::Object() {
};

void Object::render() {
	bool colorCheck = color.empty();
	glBegin(GL_TRIANGLES);
	for (unsigned int fIndex = 0; fIndex < face.size(); fIndex++) {
		for (unsigned int counter = 0; counter < 3; counter++) {
			unsigned int vIndex = face[fIndex]->v[counter] - 1;
			unsigned int nIndex = face[fIndex]->n[counter] - 1;

			GLdouble r = ((double) rand() * 1.0 / (double) RAND_MAX);
			GLdouble g = ((double) rand() * 1.0 / (double) RAND_MAX);
			GLdouble b = ((double) rand() * 1.0 / (double) RAND_MAX);


				if (colorCheck)
					glColor3f(r, g, b);
				else
					glColor3f(color[vIndex]->get(0), color[vIndex]->get(1), color[vIndex]->get(2));

				glNormal3d(normal[nIndex]->get(0), normal[nIndex]->get(1), normal[nIndex]->get(2));
				glVertex3d(vertex[vIndex]->get(0), vertex[vIndex]->get(1), vertex[vIndex]->get(2));
		}
	}
	glEnd();
}

Bs Object::update() {
	return Bs();
}

void Object::pushVertex(const GLdouble& v1, const GLdouble& v2, const GLdouble& v3) {
	Vector3d* v = new Vector3d(v1, v2, v3);
	vertex.push_back(v);
}

void Object::pushNormal(const GLdouble& n1, const GLdouble& n2, const GLdouble& n3) {
	Vector3d* n = new Vector3d(n1, n2, n3);
	n->normalize();
	normal.push_back(n);
}

void Object::pushFace(const unsigned int& v1, const unsigned int& v2, const unsigned int& v3,
	const unsigned int& n1, const unsigned int& n2, const unsigned int& n3) {
	Face* f = new Face();

	f->v[0] = v1;
	f->v[1] = v2;
	f->v[2] = v3;
	f->n[0] = n1;
	f->n[1] = n2;
	f->n[2] = n3;

	face.push_back(f);
}

void Object::pushColor(const GLdouble& r, const GLdouble& g, const GLdouble& b) {
	Vector3d* c = new Vector3d(r, g, b);
	color.push_back(c);
}

void Object::setMid() {
	GLdouble x_max = vertex[0]->get(0);
	GLdouble x_min = vertex[0]->get(0);
	GLdouble y_max = vertex[0]->get(1);
	GLdouble y_min = vertex[0]->get(1);
	GLdouble z_max = vertex[0]->get(2);
	GLdouble z_min = vertex[0]->get(2);
	for (unsigned int i = 0; i < vertex.size(); ++i) {
		if (vertex[i]->get(0) > x_max)
			x_max = vertex[i]->get(0);
		if (vertex[i]->get(0) < x_min)
			x_min = vertex[i]->get(0);
		if (vertex[i]->get(1) > y_max)
			y_max = vertex[i]->get(1);
		if (vertex[i]->get(1) < y_min)
			y_min = vertex[i]->get(1);
		if (vertex[i]->get(2) > z_max)
			z_max = vertex[i]->get(2);
		if (vertex[i]->get(2) < z_min)
			z_min = vertex[i]->get(2);
	}

	cerr << "X(min, max) = (" << x_min << ", " << x_max << ")" << endl;
	cerr << "Y(min, max) = (" << y_min << ", " << y_max << ")" << endl;
	cerr << "Z(min, max) = (" << z_min << ", " << z_max << ")" << endl;

	this->minMax[0] = x_max;
	this->minMax[1] = x_min;
	this->minMax[2] = y_max;
	this->minMax[3] = y_min;
	this->minMax[4] = z_max;
	this->minMax[5] = z_min;
	this->mid[0] = (x_max + x_min) / 2.0;
	this->mid[1] = (y_max + y_min) / 2.0;
	this->mid[2] = (z_max + z_min) / 2.0;
}

const GLdouble* Object::getMid() {
	return mid;
}

void Object::setScalar(const GLdouble& windowSize) {
	GLdouble diffX = minMax[0] - minMax[1];
	GLdouble diffY = minMax[2] - minMax[3];
	GLdouble diffZ = minMax[4] - minMax[5];

	scalar =  windowSize / diffX;
	if (diffY > diffX)
		scalar = windowSize / diffY;
	if (diffZ > diffY)
		scalar = windowSize / diffZ;

	scalar *= 0.7;
}

const GLdouble Object::getScalar() {
	return scalar;
}

Object::~Object() {
}
