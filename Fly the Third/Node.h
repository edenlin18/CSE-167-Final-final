#ifndef _NODE_H_
#define _NODE_H_

#include "Matrix4d.h"
#include "FrustumCulling.h"
#include "GL\glut.h"

struct Bs{
	Vector3d center;
	double radius;
	Bs(){};
	Bs(Vector3d c, double r){
		center = c;
		radius = r;
	}
	Bs merge(Bs & other){
		double d = (center - other.center).magnitude();
		double diff = abs(radius - other.radius);
		if (d > diff){			// intersects
			return Bs((center + other.center).scale(0.5), (d + radius + other.radius) / 2);
		}
		else{
			return radius > other.radius ? Bs(center, radius) : other;
		}
	}
};

class Node{
public :
	Node() {};
	virtual void draw(Matrix4d &) = 0; 
	virtual void draw(Matrix4d &, FrustumCulling &) = 0;
	virtual Bs update() = 0;
	virtual void drawBS(Matrix4d & m){
		Matrix4d t;
		t.makeTranslate(bs.center[0], bs.center[1], bs.center[2]);
		glMatrixMode(GL_MODELVIEW);
		glColor3d(1, 0, 1);
		glLoadMatrixd((m * t).getPointer());
		glutWireSphere(bs.radius, 30, 30);
	};

protected:
	Bs bs;
};


#endif