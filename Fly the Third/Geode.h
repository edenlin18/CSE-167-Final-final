#ifndef _GEODE_H_
#define _GEODE_H_

#include "Node.h"
#include <GLee.h>
#include <GL/glut.h>

#include <iostream>
using namespace std;

class Geode : public Node{
public:
	Geode(){};
	void draw(Matrix4d & m){

		GLuint error = glGetError();
		if (error != GL_NO_ERROR) {
		//	cerr << hex << error << " super before" << endl;
			//system("PAUSE");
		}

		// Using temp, m has to be multipled by BOTH the Modelview and Texture matrix
		Matrix4d temp;
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glGetDoublev(GL_MODELVIEW_MATRIX, temp.getPointer());
		glLoadMatrixd((temp * m).getPointer());
		

		glMatrixMode(GL_TEXTURE);
		glActiveTextureARB(GL_TEXTURE7);
		glPushMatrix();
		glGetDoublev(GL_TEXTURE_MATRIX, temp.getPointer());

		glLoadMatrixd((temp * m).getPointer());

		error = glGetError();
		if (error != GL_NO_ERROR) {
		//	cerr << hex << error << " before" << endl;
			//system("PAUSE");
		}

		render();

		error = glGetError();
		if (error != GL_NO_ERROR) {
			//cerr << hex << error << " after" << endl;
			//system("PAUSE");
		}

		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
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