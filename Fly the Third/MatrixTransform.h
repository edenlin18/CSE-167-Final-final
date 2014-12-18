#ifndef _MATRIXTRANSFORM_H_
#define _MATRIXTRANSFORM_H_

#include "Group.h"
#include "Matrix4d.h"
#include "Vector4d.h"

class MatrixTransform : public Group{
public:
	MatrixTransform(Matrix4d m);
	void setMatrix(Matrix4d m);
	void draw(Matrix4d& m);
	void draw(Matrix4d& m, FrustumCulling &);
	Bs update();
	void setBS(Vector4d, Vector4d);
	void drawBS(Matrix4d &);
	Matrix4d getMatrix();
	double getRadius();
	Vector3d getCenter();
private:
	Matrix4d matrix;
	Vector4d* center;
	Vector4d* bottomOfSphere;
	double radius;
};

#endif 