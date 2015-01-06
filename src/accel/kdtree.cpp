#include "kdtree.h"

//constructor
KDtree::KDtree(std::vector<Geometry*> prims,int d)
:max_depth(d)
{
	root = new KdNode();
	build(prims,max_depth,&root);
}


//build tree
void KDtree::build(std::vector<Geometry*> prims, int depth, KDnode *node){
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
	node->left = new KdNode();
	node->right = new KdNode();
	bbox = getBoundingBox();

	//choose split position
	axis = bbox.longest_axis();

	//for now, choose the midpoint of the longest axis
	for(int i = 0; i < primitives.size(); i++){
		BoundingBox b = primitives[i]->getBoundingBox();
		split += (b.max[axis]+b.min[axis])/2.0;
	}
	split /= primitives.size();

	std::vector<*Geometry> left_prims;
	std::vector<*Geometry> right_prims;

	//if left of midpoint
	for(int i = 0; i < primitives.size(); i++){
		if(primitives[i]->getBoundingBox().max[axis]<split)
			left_primitives.push_back(primitives[i]);
		else
			right_primitives.push_back(primitives[i]);
	}

	build(left_prims, depth--,node->left);
	build(right_prims, depth--,node->right);
	bbox = node->getBoundingBox();
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
BoundingBox KDnode::getBoundingBox(){
	std::vector<Geometry*>::iterator g;//iterator
	BoundingBox b = (*primitives.begin())->getBoundingBox();

	for( g = primitives.begin(); g != primitives.end(); ++g ) 
	{
		b = b.join((*g)->getBoundingBox());
	}
	return b;
}