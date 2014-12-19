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
	grow1.makeTranslate(0, 3, 0);
	grow2.makeTranslate(0, 1, 0);
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
		for (int i = 0; i < (iterations - ls->getGeneration()); i++)
			ls->generate();
		generation = iterations;
		s = ls->getCurrent();
	}
	MatrixTransform * root = new MatrixTransform(Matrix4d());
	Vector3d leafColor(25.0 / 256.0, 156.0 / 256.0, 42.0 / 256.0);
	TurtleInfo current;
	current.color = Vector3d(48.0 / 256.0, 24.0 / 256.0, 0.0 / 256.0);
	current.dr = dr;
	current.mt = root;
	current.radius = radius;
	current.length = radius / 2;
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
			/// using new element
			MatrixTransform * m;
			grow1.makeTranslate(0, current.length, 0);
			if (current.radius / current.length > 0.002){
				current.mt->addChild(new Cylinder(current.radius, current.radius * current.dr, current.length, 20, current.color));
			}
			else{
				current.mt->addChild(new Line(current.radius, current.length, current.color));
			}
			m = new MatrixTransform(grow1);
			/*using home made cylinder
			MatrixTransform * m;
			if (current.radius > 0.2){
				current.mt->addChild(new Cylinder(current.radius, current.radius * current.dr, 3, 20, current.color));
				m = new MatrixTransform(grow1);
			}	
			else{
				current.mt->addChild(new Line(current.radius, 1, current.color));
				m = new MatrixTransform(grow2);
			}
			*/	
			//current.mt->addChild(new Sphere(current.radius, 20, 20, current.color, draw::SOLID));
			//current.mt->addChild(new Disk(0, current.radius, 20, 20, current.color));

			current.mt->addChild(m);
			current.mt = m;
			if (current.radius / current.length > 0.02)
				current.radius = current.dr * current.radius;
			//*/
			/*  using gl line
			current.mt->addChild(new Line(current.radius, 1, current.color));
			MatrixTransform * m = new MatrixTransform(grow);
			current.mt->addChild(m);
			current.mt = m;
			current.radius = current.dr * current.radius;
			*/
			break;
		}
		case 'L':{
			double random = (double)rand() / RAND_MAX;
			if (random < 0.05)
				current.mt->addChild(new Sphere(0.5, 20, 20, Vector3d(1, 0, 0), draw::SOLID));
			else
				current.mt->addChild(new Leaf(0.6, 0.8, leafColor));
			//current.mt->addChild(new Sphere(1, 20, 20, Vector3d(0, current.radius / 4, 0), draw::SOLID));
			//current.mt->addChild(new Cube(0.05, Vector3d(0, 1, 0), draw::SOLID));
			break;
		}
			
		case '[':
			st.push(current);
			current.length = current.length * current.dr;
			break;
		case ']':
			current = st.top();
			st.pop();
			break;
		case '!':	// decrease current thickness
			if (current.radius > 0.2)
				current.radius = current.dr * current.radius;
			break;
		case '?':	// increase current thickness
			current.radius = current.radius / current.dr;
			break;
		case '<':	// increase current length
			current.length = current.length / current.dr;
			break;
		case '>':	// decrease current length
			current.length = current.length * current.dr;
			break;
		}
	}
	return root;
}