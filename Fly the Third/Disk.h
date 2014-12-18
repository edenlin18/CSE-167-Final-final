#ifndef DISK_H_
#define DISK_H_

#include "Geode.h"
#include "Const.h"
#include "Vector3d.h"
#include "Material.h"

class Disk : public Geode{
public:
	Disk(double inner, double outter,int slices,int loops, Vector3d color, Material *ma = NULL);
	Bs update();
private:
	void render();
	double inner, outer;
	int slices;
	int loops;
	Vector3d color;
	Material *material;
	GLUquadricObj *obj;
};

#endif