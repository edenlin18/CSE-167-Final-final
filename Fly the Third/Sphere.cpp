#include "Sphere.h"

Sphere::Sphere(double r, int sl, int st, Vector3d c, draw::mode m, Material *ma){
	radius = r;
	slices = sl;
	stacks = st;
	color = c;
	mode = m;
	material = ma;
	bs.radius = radius;
	bs.center = Vector3d(0, 0, 0);
}

void Sphere::setColor(Vector3d c){
	color = c;
}

void Sphere::render(){
	glDisable(GL_CULL_FACE);
	if (material != NULL){
		material->apply();
	}	
	else
		glColor3d(color[0], color[1], color[2]);
	switch (mode){
	case draw::SOLID:
		glutSolidSphere(radius, slices, stacks);
		break;
	case draw::WIRE:
		glutWireSphere(radius, slices, stacks);
		break;
	}
	
}

Bs Sphere::update(){
	
	return bs;
}