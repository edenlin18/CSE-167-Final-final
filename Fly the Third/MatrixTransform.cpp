#include "MatrixTransform.h"
#include "GL\glut.h"
using namespace std;


MatrixTransform::MatrixTransform(Matrix4d m){
	matrix = m;
	center = new Vector4d();
	bottomOfSphere = new Vector4d();
}

void MatrixTransform::setMatrix(Matrix4d m){
	matrix = m;
}

Matrix4d MatrixTransform::getMatrix(){
	return matrix;
}

void MatrixTransform::draw(Matrix4d & m){
	Matrix4d mm = m * matrix;
	Group::draw(mm);
}

void MatrixTransform::draw(Matrix4d & m, FrustumCulling & fc){
	Matrix4d mm = m * matrix;
	Group::draw(mm,fc);
}

Bs MatrixTransform::update(){
	bs = (*(children.begin()))->update();
	for (list<Node*>::const_iterator it = children.begin(); it != children.end(); ++it){
		bs = bs.merge((*it)->update());
	}
	Vector4d c(bs.center[0], bs.center[1], bs.center[2], 1);
	c = matrix * c;
	bs.center = c.getVector3d();
	double x = matrix.get(0, 0);
	double y = matrix.get(1, 1);
	double z = matrix.get(2, 2);
	if (x != 1.0 || y != 1.0 || z != 1.0){
		double a = (x >= y ? x : y);
		double b = (a >= z ? a : z);
		bs.radius = b * bs.radius;
	}
	return bs;
}

double MatrixTransform::getRadius(){
	return radius;
}

Vector3d MatrixTransform::getCenter(){
	return center->getVector3d();
}

void MatrixTransform::setBS(Vector4d c, Vector4d p){
	(*center) = c;
	(*bottomOfSphere) = p;
}


void MatrixTransform::drawBS(Matrix4d & m){
	/*
	Vector3d c = (matrix * (*center)).getVector3d();
	Vector3d p = (matrix * (*bottomOfSphere)).getVector3d();
	Vector3d d = c - p;
	double r = d.magnitude();
	Matrix4d t;
	t.makeTranslate(d[0], d[1], d[2]);
	//c.print("center:");
	//p.print("bottom:");
	//d.print("displace: ");
	//Debug::debug("radius: ", r);

	glMatrixMode(GL_MODELVIEW);
	glColor3d(1, 0, 1);
	glLoadMatrixd((matrix * t).getPointer());
	glutWireSphere(r, 10, 10);
	//sphereMatrix.print("sphereMatrix:");
	//Debug::debug("radius", radius);

	*/
	Group::drawBS(m);
	for (list<Node*>::const_iterator it = children.begin(); it != children.end(); ++it){
		(*it)->drawBS(m * matrix);
	}

}
