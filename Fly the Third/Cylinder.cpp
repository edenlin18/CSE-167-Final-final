#include "Cylinder.h"

#include "Sphere.h"

Cylinder::Cylinder(double base_r, double top_r, double height, int resolution, Vector3d color, Material *ma){
	this -> base_r = base_r;
	this -> top_r = top_r;
	this -> height = height;
	this->resolution = resolution;
	this -> color = color;
	material = ma;
	top = new Vector4d*[resolution - 1];
	bottom = new Vector4d*[resolution - 1];
	top[0] = new Vector4d(0, height, top_r);
	bottom[0] = new Vector4d(0, 0, base_r);
	Matrix4d r;
	r.makeRotateY(360.0 / resolution);
	for (int i = 1; i < resolution - 1; i++){
		top[i] = new Vector4d(r * (*top[i - 1]));
		bottom[i] = new Vector4d(r * (*bottom[i - 1]));
	}
}

void Cylinder::render(){
	if (material != NULL){
		material->apply();
	}
	else
		glColor3d(color[0], color[1], color[2]);
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < resolution - 1; i++){
		glVertex3f((*top[i])[0], (*top[i])[1], (*top[i])[2]);
		glVertex3f((*bottom[i])[0], (*bottom[i])[1], (*bottom[i])[2]);
	}
	glVertex3f((*top[0])[0], (*top[0])[1], (*top[0])[2]);
	glVertex3f((*bottom[0])[0], (*bottom[0])[1], (*bottom[0])[2]);
	glEnd();
}

Bs Cylinder::update(){

	return bs;
}