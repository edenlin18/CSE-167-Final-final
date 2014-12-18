#include "Airplane.h"

Airplane::Airplane(Vector3d p, Vector3d d) {
	position = p;
	direction = d;
	direction.normalize();
	init();
}

void Airplane::init() {
	root = new MatrixTransform(Matrix4d());
	root->addChild(Parser::parse_object("./resource/BiPlaneObject.object"));
	//root->addChild(Parser::parse_object("./resource/paperAirplane.object"));
	s.makeScale(0.01, 0.01, 0.01);
	t.makeTranslate(position[0], position[1], position[2]);
	//Vector3d axis = Vector3d(-1, 0, 0) * direction;
	Vector3d axis = direction * Vector3d(1, 0, 0);
	double angle = acos(direction.dot(Vector3d(1, 0, 0))) * 180 / M_PI;
	r.makeRotate(-angle, axis);
	root->setMatrix(t * s * r);
}


void Airplane::setMatrixTransformMatrix(Matrix4d& C) {
	root->setMatrix(C);
}

Vector3d Airplane::getPosition(){
	return position;
}

Matrix4d Airplane::getMatrixTransformMatrix() {
	return root->getMatrix();
}

MatrixTransform* Airplane::getRoot() {
	return root;
}

Airplane::~Airplane() {
}

void Airplane::move(Vector3d pos){
	Vector3d direction = pos - position;
	position = pos;
	direction.normalize();
	Vector3d axis = direction * Vector3d(1, 0, 0);
	axis.normalize();
	double angle = acos(direction.dot(Vector3d(1, 0, 0))) * 180 / M_PI;
	r.makeRotate(-angle, axis);
	t.makeTranslate(position[0], position[1], position[2]);
	root->setMatrix(t * s * r);
}