#ifndef LINE_H_
#define LINE_H_

#include "Geode.h"
#include "Const.h"
#include "Vector3d.h"
#include "Material.h"

class Line : public Geode{
public:
	Line(double width, double height, Vector3d color);
	Bs update();
private:
	void render();
	double width, height;
	Vector3d color;
};

#endif