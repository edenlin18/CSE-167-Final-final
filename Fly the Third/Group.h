#ifndef _GROUP_H_
#define _GROUP_H_

#include "Node.h"
#include <list>


class Group : public Node{
public :
	Group(){};
	~Group();
	virtual void draw(Matrix4d &);
	virtual void draw(Matrix4d &, FrustumCulling &);
	virtual Bs update() = 0;
	void addChild(Node *);
	void removeChild(Node *);
	void remove();

protected :
	std::list<Node*> children;
}; 

#endif