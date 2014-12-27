#include "kdtree.h"
//make sure kdtree nodes are individual
KDnode::KDnode(){	

}

KDnode::~KDnode(){
	
}

KDnode* KDnode::build(vector<Geometry*> prims,int depth){

	//make leaf
	if(depth == 0)
		return NULL;

	//make interior node
	

	return NULL;
}

//return node bounding box
BoundingBox getBoundingBox(){
	std::vector<Geometry*>::iterator g;//iterator
	
	BoundingBox b = objects.begin().getBoundingBox();
	
	for( g = objects.begin(); g != objects.end(); ++g ) 
	{
		b = b.join((*g)->getBoundingBox());
	}
}