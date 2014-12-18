#ifndef CYLINDER_H_
#define CYLINDER_H_

#include "Geode.h"
#include "Const.h"
#include "Vector3d.h"
#include "Material.h"

class Cylinder : public Geode{
public:
	Cylinder(double base_r, double top_r, double height, int resolution, Vector3d color, Material *ma = NULL);
	Bs update();
private:
	void render();
	double base_r, top_r, height;
	int resolution;
	Vector3d color;
	Material *material;
	Vector4d ** top;
	Vector4d ** bottom;
};

#endif