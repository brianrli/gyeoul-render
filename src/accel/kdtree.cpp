#include "kdtree.h"

//constructor
KDtree::KDtree(std::vector<Geometry*> prims,int d)
:max_depth(d)
{
	root = new KDnode();
	build(prims,max_depth,root);
}


//build tree
void KDtree::build(std::vector<Geometry*> &prims, int& depth, KDnode* node){
	node->primitives = prims;

	primitives = prims;
	//=====[make leaf]=====
	if(depth == 0)
		return;

	//no primitives
	if(primitives.size() == 0){
		return;
	}

	if(primitives.size()==1){
		return;
	}

	//=====[ make interior node ]=====
	node->left = new KDnode();
	node->right = new KDnode();
	node->bbox = node->getBoundingBox();

	//choose split position
	node->axis = node->bbox->longest_axis();
	int axis = node->axis;

	//for now, choose the midpoint of the longest axis
	for(size_t i = 0; i < primitives.size(); i++){
		BoundingBox b = primitives[i]->getBoundingBox();
		node->split += (b.max[axis]+b.min[axis])/2.0;
	}
	node->split /= primitives.size();

	std::vector<Geometry*> left_primitives;
	std::vector<Geometry*> right_primitives;
	

	//if left of midpoint
	for(size_t i = 0; i < primitives.size(); i++){
		if(primitives[i]->getBoundingBox().max[axis]<node->split)
			left_primitives.push_back(primitives[i]);
		else
			right_primitives.push_back(primitives[i]);
	}

	depth--;
	build(left_primitives, depth,node->left);
	build(right_primitives, depth,node->right);
	
	return;
}

//make sure kdtree nodes are individual
KDnode::KDnode(){	
	left = NULL;
	right = NULL;
}

KDnode::~KDnode(){
	// delete (*left);
	// delete(*right);
}

//return node bounding box
//change bbox to this bounding box
BoundingBox* KDnode::getBoundingBox(){
	std::vector<Geometry*>::iterator g;//iterator
	BoundingBox b = (*primitives.begin())->getBoundingBox();

	for( g = primitives.begin(); g != primitives.end(); ++g ) 
	{
		b = b.join((*g)->getBoundingBox());
	}

	BoundingBox *bbx = new BoundingBox(b);
	return bbx;
}