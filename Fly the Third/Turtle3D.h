#ifndef TURTLE3D_H_
#define TURTLE3D_H_
#include "lstring.h"
#include "Vector4d.h"
#include "Matrix4d.h"
#include "MatrixTransform.h"
#include <stack>

#define MyStack stack<TurtleInfo>
#define Branch 'F'
#define Leave 'L'
using namespace std;

enum COLOR{RED, GREEN, BLUE, WHITE, BLACK};
struct TurtleInfo{
	Vector3d color;
	double radius;
	double dr;
	MatrixTransform * mt;
};



class Turtle3D{
public:
	Turtle3D(string ax, string var, double an, double ra, double _dr);
	void addRule(char, string , double);
	MatrixTransform * generate(int iterations);
private :
	double angle;		// rotation angle
	double dr;			// width decreasing rate
	double radius;
	int generation;
	Lstring* ls;
	MyStack st;
	string axiom;
	Matrix4d left, right, pitchUp, pitchDown, rollLeft, rollRight, grow;

};

#endif