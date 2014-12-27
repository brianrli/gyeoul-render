#include "kdtree.h"

void Kdtree::build(){

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

KDnode* KDnode::build(int depth){
	//=====[make leaf]=====
	if(depth == 0)
		return NULL;

	//no primitives
	if(primitives.size() == 0){
		return NULL;
	}

	//=====[ make interior node ]=====
	left = new KDnode(); //left child
	right = new KDnode(); //right child

	//choose split position
	axis = bbox.longest_axis();

	//for now, choose the midpoint of the longest axis
	for(int i = 0; i < primitives.size(); i++){
		BoundingBox b = primitives[i]->getBoundingBox();
		split += (b.max[axis]+b.min[axis])/2.0;
	}
	split /= primitives.size();

	//if left of midpoint
	for(int i = 0; i < primitives.size(); i++){
		if(primitives[i]->getBoundingBox().max[axis]<split)
			left->primitives.push_back(primitives[i]);
		else
			right->primitives.push_back(primitives[i]);
	}

	left->build(depth--);
	right->build(depth--);
	return this;
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