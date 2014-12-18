#ifndef CYLINDER_H_
#define CYLINDER_H_

#include "Geode.h"
#include "Const.h"
#include "Vector3d.h"
#include "Material.h"

class Cylinder : public Geode{
public:
	Cylinder(double base, double top, double height, int slices, int stacks, Vector3d color, Material *ma = NULL);
	Bs update();
private:
	void render();
	double base, top, height;
	int slices;
	int stacks;
	Vector3d color;
	Material *material;
	GLUquadricObj *obj;
};

#endif