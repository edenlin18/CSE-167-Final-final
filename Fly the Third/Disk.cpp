#include "Disk.h"

Disk::Disk(double _inner, double _outer, int _slices, int _loops, Vector3d _color, Material *ma){
	inner = _inner;
	outer = _outer;
	slices = _slices;
	loops = _loops;
	color = _color;
	material = ma;
	obj = gluNewQuadric();
}

void Disk::render(){
	if (material != NULL){
		material->apply();
	}
	else
		glColor3d(color[0], color[1], color[2]);
	gluDisk(obj, inner, outer, slices, loops);
}

Bs Disk::update(){

	return bs;
}