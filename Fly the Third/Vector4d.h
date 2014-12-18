#ifndef _VECTOR4_H_
#define _VECTOR4_H_
#include <Windows.h>

#include "GLee.h"
#include "GL/glut.h"
#include "Vector3d.h"
#include "Matrix4d.h"

class Matrix4d;

class Vector4d
{
protected:
	GLdouble v[4];   // matrix elements

public:
	Vector4d();        // constructor
	Vector4d(GLdouble, GLdouble, GLdouble);
	Vector4d(GLdouble, GLdouble, GLdouble, GLdouble);
	void set(int index, GLdouble value);
	GLdouble get(int index);
	Vector4d& operator=(Vector4d&);
	Vector4d operator*(Matrix4d&);
	double operator*(Vector4d&);
	GLdouble operator[](int index);
	Vector4d operator+(Vector4d & another);
	void add(Vector4d & another);
	Vector4d operator-(Vector4d & another);
	Vector3d getVector3d();
	void substract(Vector4d & another);
	void dehomogenize(); 
	void print();

};

#endif