#ifndef LEAF_H_
#define LEAF_H_

#include "Geode.h"
#include "Const.h"
#include "Vector3d.h"
#include "Material.h"

class Leaf : public Geode{
public:
	Leaf(double width, double height, Vector3d color, Material * m = NULL);
	Bs update();
private:
	void render();
	double width, height;
	Vector3d color;
	Material * m;
};

#endif