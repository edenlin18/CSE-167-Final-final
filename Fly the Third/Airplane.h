#ifndef _AIRPLANE_H_
#define _AIRPLANE_H_

#include "MatrixTransform.h"
#include "Object.h"
#include "Parser.h"

class Airplane {
public:
	Airplane(Vector3d position, Vector3d direction);
	void init();
	void setMatrixTransformMatrix(Matrix4d&);
	void move(Vector3d);
	Matrix4d getMatrixTransformMatrix();
	MatrixTransform* getRoot();
	~Airplane();

private:
	Vector3d position;
	Vector3d direction;
	MatrixTransform* root;
	Matrix4d s;
	Matrix4d r;
	Matrix4d t;
	Matrix4d temp;
};

#endif