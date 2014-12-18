#include "Group.h"

using namespace std;


Group::~Group(){
	for (list<Node*>::const_iterator it = children.begin(); it != children.end(); ++it)
		delete *it;
}

void Group::addChild(Node * n){
	children.push_back(n);
}

void Group::removeChild(Node * n){
	children.remove(n);	
}

void Group::draw(Matrix4d & m){
	for (list<Node*>::const_iterator it = children.begin(); it != children.end(); ++it)
		(*it)->draw(m); 
}

void Group::draw(Matrix4d & m, FrustumCulling & fc){
	switch (fc.sphereInFrustum(bs.center, bs.radius)){
	case FrustumCulling::INSIDE:
		draw(m);
		break;
	case FrustumCulling::INTERSECT:
		for (list<Node*>::const_iterator it = children.begin(); it != children.end(); ++it)
			(*it)->draw(m, fc);
		break;
	}
}
