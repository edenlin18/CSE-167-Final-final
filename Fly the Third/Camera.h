#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Matrix4d.h"
#include "Vector3d.h"

enum Direction{FORWARD, BACKWARD, LEFTWARD, RIGHTWARD, UPWARD, DOWNWARD};
class Camera{
protected:
	Vector3d up;			// up-vector
	Vector3d e;				// projection
	Vector3d d;				// look at
	Matrix4d m;				// camera inverse

	void computeMatrix();
	void setCamera(double eyeX, double eyeY, double eyeZ,
		double centerX, double centerY, double centerZ,
		double upX, double upY, double upZ);

public:
	Camera(Vector3d e, Vector3d d, Vector3d up);
	Camera(double eyeX, double eyeY, double eyeZ,
		double centerX, double centerY, double centerZ,
		double upX, double upY, double upZ);
	Matrix4d getMatrix(); 
	void move(Direction d, double delta);
	void move(Vector3d);		// move according to displacement vector
	void rotate(Vector3d axis, double angle);
	Vector3d getEye();
	Vector3d getLookAt();
	Vector3d getUp();

};

#endif