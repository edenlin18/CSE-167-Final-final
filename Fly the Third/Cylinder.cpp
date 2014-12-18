#include "Cylinder.h"

#include "Sphere.h"

Cylinder::Cylinder(double _base, double _top, double h, int sl, int st, Vector3d c, Material *ma){
	base = _base;
	top = _top;
	height = h;
	slices = sl;
	stacks = st;
	color = c;
	material = ma;
	obj = gluNewQuadric();
}

void Cylinder::render(){
	if (material != NULL){
		material->apply();
	}
	else
		glColor3d(color[0], color[1], color[2]);
	gluCylinder(obj, base, top, height, slices, stacks);

}

Bs Cylinder::update(){

	return bs;
}