#ifndef PIECEWISEBEZIERCURVE_H_
#define PIECEWISEBEZIERCURVE_H_

#include "Matrix4d.h"
#include "Vector3d.h"
#include "MatrixTransform.h"
#include "Sphere.h"


class PiecewiseBezierCurve{

public:
	PiecewiseBezierCurve(int, Vector3d *, int, bool debug = false);
	Vector3d compute(double t);
	bool setCp(int index, Vector3d cp);
	Vector3d getCp(int index);
	void render(Matrix4d);
	void selectNext();
	void selectPrev();
	int getSelected();
	void deSelect();
private :
	int N;
	int resolution;
	bool debug;
	int selected;
	Vector3d * cp;	// array of control points
	Matrix4d * Cs;	// array of matrix for computing x(t)
	Matrix4d Bez;
	MatrixTransform *root;
	Sphere ** cpBall;
	MatrixTransform ** ballPosition;
	Matrix4d t;

	void recalulateBez(int index);
};

#endif