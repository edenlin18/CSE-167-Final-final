#pragma once

#include <iostream>
#include <vector>
#include "Geode.h"
#include "Vector3d.h"

using namespace std;

struct Face {
	unsigned int v[3];
	unsigned int n[3];
};

class Object : public Geode {
	public:
	Object();
	void render();
	Bs update();
	void pushVertex(const GLdouble&, const GLdouble&, const GLdouble&);
	void pushNormal(const GLdouble&, const GLdouble&, const GLdouble&);
	void pushFace(const unsigned int&, const unsigned int&, const unsigned int&, 
		const unsigned int&, const unsigned int&, const unsigned int&);
	void pushColor(const GLdouble&, const GLdouble&, const GLdouble&);
	void setMid();
	const GLdouble* getMid();
	void setScalar(const GLdouble&);
	const GLdouble getScalar();
	~Object();

	protected:
	vector<Vector3d*> vertex;
	vector<Vector3d*> normal;
	vector<Vector3d*> color;
	vector<Face*> face;
	GLdouble minMax[6];
	GLdouble mid[3];
	GLdouble scalar;
};