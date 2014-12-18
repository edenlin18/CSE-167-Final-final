#include <math.h>
#include <iostream>
#include "Matrix4d.h"

Matrix4d::Matrix4d(double m00, double m10, double m20, double m30,
	double m01, double m11, double m21, double m31,
	double m02, double m12, double m22, double m32,
	double m03, double m13, double m23, double m33){
	m[0][0] = m00;
	m[1][0] = m10;
	m[2][0] = m20;
	m[3][0] = m30;

	m[0][1] = m01;
	m[1][1] = m11;
	m[2][1] = m21;
	m[3][1] = m31;

	m[0][2] = m02;
	m[1][2] = m12;
	m[2][2] = m22;
	m[3][2] = m32;

	m[0][3] = m03;
	m[1][3] = m13;
	m[2][3] = m23;
	m[3][3] = m33;
}

Matrix4d::Matrix4d()
{
  for (int i=0; i<4; ++i)
  {
    for (int j=0; j<4; ++j)
    {
      m[i][j] = 0;
    }
  }
  for (int i = 0; i < 4; i++)
	  m[i][i] = 1;
} 

Matrix4d& Matrix4d::operator=(const Matrix4d& m2)
{
  if (this != &m2)
  {
    for (int i=0; i<4; ++i)
    {
      for (int j=0; j<4; ++j)
      {
        m[i][j] = m2.m[i][j];
      } 
    }
  }
  return *this;
}

// return pointer to matrix elements
double* Matrix4d::getPointer()
{
  return &m[0][0];
}

// set matrix to identity matrix
void Matrix4d::identity()
{
  for (int i=0; i<4; ++i)
  {
    for (int j=0; j<4; ++j)
    {
        m[i][j] = 0.0;
    }
  }
  for (int j = 0; j<4; ++j)
  {
	  m[j][j] = 1.0;
  }
}

// transpose the matrix (mirror at diagonal)
void Matrix4d::transpose()
{
  double temp;
  for (int i=0; i<4; ++i)
  {
    for (int j=0; j<i; ++j)
    {
		temp = m[j][i];
		m[j][i] = m[i][j];
		m[i][j] = temp;
    }
  }
}

Matrix4d Matrix4d::operator*(const Matrix4d& other){
	Matrix4d result;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++){
		result.m[i][j] = m[0][j] * other.m[i][0] + m[1][j] * other.m[i][1] + m[2][j] * other.m[i][2] + m[3][j] * other.m[i][3];
		}
	return result;
}

Vector4d Matrix4d::operator*(Vector4d& other){
	Vector4d result;
	for (int i = 0; i < 4; i++){
		result.set(i, other[0] * m[0][i] + other[1] * m[1][i] + other[2] * m[2][i] + other[3] * m[3][i]);
	}
	return result;
}

void Matrix4d::multiply(const Matrix4d& other){
	*this = *this * other;
}

void Matrix4d::makeRotateX(double angle){
	angle = angle / 180.0 * M_PI;
	identity();
	m[1][1] = cos(angle);
	m[2][1] = -sin(angle);
	m[1][2] = sin(angle);
	m[2][2] = cos(angle);
}

// Creates a rotation matrix which rotates about the y axis.
// angle is expected in degrees
void Matrix4d::makeRotateY(double angle)
{
	angle = angle / 180.0 * M_PI;  // convert from degrees to radians
	identity();
	m[0][0] = cos(angle);
	m[0][2] = -sin(angle);
	m[2][0] = sin(angle);
	m[2][2] = cos(angle);
}

void Matrix4d::makeRotateZ(double angle){
	angle = angle / 180.0 * M_PI;
	identity();
	m[0][0] = cos(angle);
	m[1][0] = -sin(angle);
	m[0][1] = sin(angle);
	m[1][1] = cos(angle);
}

void Matrix4d::makeRotate(double angle, Vector3d & axis){
	angle = angle / 180.0 * M_PI;
	identity();
	double s = sin(angle);
	double c = cos(angle);
	double t = 1 - c;
	double x = axis[0];
	double y = axis[1];
	double z = axis[2];
	double xx = x * x;
	double yy = y * y;
	double zz = z * z;
	double xy = x * y;
	double xz = x * z;
	double yz = y * z;
	/*
	m[0][0] = 1 + t*(xx - 1);
	m[0][1] = t*xy + s*z;
	m[0][2] = t*xz - s*y;
	m[1][0] = t*xy - s*z;
	m[1][1] = 1 + t *(yy-1);
	m[1][2] = t*yz + s*x;
	m[2][0] = t*xz + s*y;
	m[2][1] = t*yz - s*x;
	m[2][2] = 1 + t*(zz - 1);
	*/
	m[0][0] = t * xx + c;
	m[0][1] = t*xy + s*z;
	m[0][2] = t*xz - s*y;

	m[1][0] = t*xy - s*z;
	m[1][1] = t*yy + c;
	m[1][2] = t*yz + s*x;

	m[2][0] = t*xz + s*y;
	m[2][1] = t*yz - s*x;
	m[2][2] = t*zz + c;
	/*

	angle = angle / 180.0 * M_PI;
	identity();
	double s = sin(angle);
	double c = 1 - cos(angle);
	double x = axis[0];
	double y = axis[1];
	double z = axis[2];
	double xx = x * x;
	double yy = y * y;
	double zz = z * z;
	double xy = x * y;
	double xz = x * z;
	double yz = y * z;
	
	m[0][0] = 1 + c * (xx - 1);
	m[0][1] = z * s + c * xy;
	m[0][2] = -y * s + c * xz;
	m[1][0] = -z * s + c * xy;
	m[1][1] = 1 + c * (yy - 1);
	m[1][2] = x * s + c * yz;
	m[2][0] = y * s + c * xz;
	m[2][1] = -x * s + c * yz;
	m[2][2] = 1 + c * (zz - 1);
	
	m[0][0] = 1 + c * (xx - 1);
	m[1][0] = z * s + c * xy;
	m[2][0] = -y * s + c * xz;
	m[0][1] = -z * s + c * xy;
	m[1][1] = 1 + c * (yy - 1);
	m[2][1] = x * s + c * yz;
	m[0][2] = y * s + c * xz;
	m[1][2] = -x * s + c * yz;
	m[2][2] = 1 + c * (zz - 1);
	*/
}

void Matrix4d::makeScale(double sx, double sy, double sz){
	identity();
	m[0][0] = sx;
	m[1][1] = sy;
	m[2][2] = sz;
}

void Matrix4d::makeTranslate(double tx, double ty, double tz){
	identity();
	m[3][0] = tx;
	m[3][1] = ty;
	m[3][2] = tz;
}

void Matrix4d::print(std::string comment){
	std::cout << comment << std::endl;
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++)
			std::cout << m[j][i] << " ";
		std::cout << std::endl;
	}
}

void Matrix4d::set(int x, int y, double e){
	m[x][y] = e;
}

double Matrix4d::get(int x, int y){
	return m[x][y];
}