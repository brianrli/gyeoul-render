#ifndef KDTREE_H
#define KDTREE_H
#include "../scene/scene.h"

using namespace std;

class Geometry;
class BoundingBox;

//node
class KDnode{

public:
	//functions
	KDnode();
	~KDnode();
	bool intersect(const ray&r, isect&i, bool&have_one);
	BoundingBox* getBoundingBox();

	//members
	BoundingBox *bbox;
	KDnode *left; //left child
	KDnode *right; //right child
	vector<Geometry*> primitives; //geometry
	float split;
	int axis;		
};

//tree
class KDtree{

public:
	KDtree();
	KDtree(std::vector<Geometry*> *prims,int d);
	~KDtree();

	void build(std::vector<Geometry*> &prims, int& depth, KDnode* node);
	bool intersect(const ray&r, isect&i);
	KDnode *root;

	int max_depth;
};

#endif
