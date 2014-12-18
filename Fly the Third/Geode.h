#ifndef _GEODE_H_
#define _GEODE_H_

#include "Node.h"
#include "GL\glut.h"

class Geode : public Node{
public:
	Geode(){};
	void draw(Matrix4d & m){
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixd(m.getPointer());
		render();
		/*
		Matrix4d temp;
		glPushMatrix();
		glGetDoublev(GL_MODELVIEW_MATRIX, temp.getPointer());
		glLoadMatrixd((temp * m).getPointer());
		render();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		*/
	};
	void draw(Matrix4d & m, FrustumCulling & fc){
		switch(fc.sphereInFrustum(bs.center, bs.radius)){
			case FrustumCulling::INSIDE:
				draw(m);
				break;
		}
	};
	virtual Bs update() = 0;
protected:
	virtual void render() = 0; 

};


#endif