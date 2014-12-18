#ifndef _FRUSTUMCULLING_H_
#define _FRUSTUMCULLING_H_

#include "Vector3d.h"
#define M_PI 3.14159265358979323846

struct Plane{
	Vector3d a, b, c, normal;
	void set3Points(Vector3d aa, Vector3d bb, Vector3d cc){
		a = aa;
		b = bb;
		c = cc;
		normal = (b - a) * (c - a);
		normal.normalize();
	}
	double distance(Vector3d p){
		return (p - a).dot(normal);
	}
};

class FrustumCulling{
private:

	enum {
		TOP = 0, BOTTOM, LEFT,
		RIGHT, NEARP, FARP 
	};

public:

	static enum { OUTSIDE, INTERSECT, INSIDE };
	Plane pl[6];
	Vector3d ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;
	double nearD, farD, ratio, angle, tang;
	double nw, nh, fw, fh;

	FrustumCulling(double angle, double ratio, double nearD, double farD, Vector3d& eye, Vector3d& l, Vector3d& up);
	~FrustumCulling();
	int pointInFrustum(Vector3d &p);
	int sphereInFrustum(Vector3d &p, float raio);
private:
	void setCamInternals(double angle, double ratio, double nearD, double farD);
	void setCamDef(Vector3d &p, Vector3d &l, Vector3d &u);
};


#endif