#ifndef KDTREE_H
#define KDTREE_H
#include "../scene/scene.h"

using namespace std;

class Geometry;
class BoundingBox;

//node
class KDnode{

public:
	KDnode();
	~KDnode();
	BoundingBox *bbox;
	KDnode *left; //left child
	KDnode *right; //right child
	Geometry *bob;

	vector<Geometry*> primitives; //geometry
	BoundingBox* getBoundingBox();
	float split;
	int axis;	
	
};

//tree
class KDtree{
public:
	KDtree();
	KDtree(std::vector<Geometry*> prims,int d);
	~KDtree();

	void build(std::vector<Geometry*> &prims, int& depth, KDnode* node);
	KDnode *root;

	std::vector<Geometry*> primitives;
	int max_depth;
};

#endif
