#include "Turtle3D.h"
#include "Cylinder.h"
#include "Disk.h"
#include "Line.h"
#include "Sphere.h"
#include "Leaf.h"
#include "Cube.h"
#include <iostream>

Turtle3D::Turtle3D(string ax, string var, double an, double ra, double _dr){
	axiom = ax;
	ls = new Lstring(axiom, var);
	angle = an;
	radius = ra;
	dr = _dr;
	left.makeRotateY(angle);
	right.makeRotateY(-angle);
	pitchUp.makeRotateX(angle);
	pitchDown.makeRotateX(-angle);
	rollLeft.makeRotateZ(angle);
	rollRight.makeRotateZ(-angle);
	grow.makeTranslate(0, 0.1, 0);
	generation = 0;
}

void Turtle3D::addRule(char c, string s, double d){
	ls->addRule(new Rule(c, s, d));
}

MatrixTransform * Turtle3D::generate(int iterations){
	string s;
	if (generation >= iterations)
		s = ls->getGeneration(iterations);
	else{
		for (int i = 0; i < iterations; i++)
			ls->generate();
		generation = iterations;
		s = ls->getCurrent();
	}
	MatrixTransform * root = new MatrixTransform(Matrix4d());
	TurtleInfo current;
	current.color = Vector3d(1, 0, 0);
	current.dr = dr;
	current.mt = root;
	current.radius = radius;
	for (int i = 0; i < s.length(); i++){
		char c = s[i];
		switch (c){
		case '+':
			current.mt->setMatrix(left * (current.mt->getMatrix()));
			break;
		case '-':
			current.mt->setMatrix(right * (current.mt->getMatrix()));
			break;
		case '&':
			current.mt->setMatrix(pitchDown * (current.mt->getMatrix()));
			break;
		case '^':
			current.mt->setMatrix(pitchUp * (current.mt->getMatrix()));
			break;
		case '\'':
			current.mt->setMatrix(rollLeft * (current.mt->getMatrix()));
			break;
		case '/':
			current.mt->setMatrix(rollRight * (current.mt->getMatrix()));
			break;
		case 'F':{
			/*
			current.mt->addChild(new Cylinder(current.radius, current.radius * current.dr, 1, 20, 20, current.color));
			//current.mt->addChild(new Sphere(current.radius, 20, 20, current.color, draw::SOLID));
			current.mt->addChild(new Disk(0, current.radius, 20, 20, current.color));
			MatrixTransform * m = new MatrixTransform(grow);
			current.mt->addChild(m);
			current.mt = m;
			current.radius = current.dr * current.radius;
			*/
			///*
			current.mt->addChild(new Line(current.radius, 1, current.color));
			MatrixTransform * m = new MatrixTransform(grow);
			current.mt->addChild(m);
			current.mt = m;
			current.radius = current.dr * current.radius;
			//*/
			break;
		}
		case 'L':{
			//current.mt->addChild(new Leaf(0.02, 0.03, Vector3d(0, 1, 0)));
			current.mt->addChild(new Sphere(0.05, 20, 20, Vector3d(0, 1, 0), draw::SOLID));
			//current.mt->addChild(new Cube(0.05, Vector3d(0, 1, 0), draw::SOLID));
			break;
		}
			
		case '[':
			st.push(current);
			current.radius = current.radius * current.radius;
			break;
		case ']':
			current = st.top();
			st.pop();
			break;
		}
	}
	return root;
}