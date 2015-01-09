#include "kdtree.h"

//constructor
KDtree::KDtree(std::vector<Geometry*> *prims,int d)
:max_depth(d)
{
	root = new KDnode();
	std::vector<Geometry*> objects = *prims;

	int i = 0;
	std::vector<Geometry*>::const_iterator g;
	for( g = objects.begin(); g != objects.end(); ++g ) {
		std::cout << i << "\n";
		i++;
	}

	build(objects,max_depth,root);
}


//build tree
void KDtree::build(std::vector<Geometry*> &prims, int& depth, KDnode* node){
	
	// std::vector<Geometry*> primitives(&prims);
	node->primitives = prims;

	std::cout << "KDtree::build\n";
	// std::cout << node->getBoundingBox()->min << "\n";
	// std::cout << node->getBoundingBox()->max << "\n";

	// primitives = prims;
	//=====[make leaf]=====
	if(depth == 0)
		return;

	//no primitives
	std::cout << node->primitives.size() << "\n";
	if(node->primitives.size() == 0){
		std::cout << "terminate\n";
		return;
	}

	if(node->primitives.size()==1){
		std::cout << "terminate1\n";
		return;
	}

	//=====[ make interior node ]=====
	node->left = new KDnode();
	node->right = new KDnode();
	node->bbox = node->getBoundingBox();

	//choose split position
	node->axis = node->bbox->longest_axis();
	int axis = node->axis;
	std::cout << "axis is: " << axis << "\n";

	//for now, choose the midpoint of the longest axis
	for(size_t i = 0; i < node->primitives.size(); i++){
		BoundingBox b = node->primitives[i]->getBoundingBox();
		std::cout << b.max << "\n";
		std::cout << b.min << "\n";
		std::cout << i << ": " << (b.max[axis]+b.min[axis])/2.0 << "\n";
		node->split += (b.max[axis]+b.min[axis])/2.0;
	}
	node->split /= node->primitives.size();

	std::cout << "split is: " << node->split << "\n";

	std::vector<Geometry*> left_primitives;
	std::vector<Geometry*> right_primitives;

	//if left of midpoint
	for(size_t i = 0; i < node->primitives.size(); i++){
		if(node->primitives[i]->getBoundingBox().max[axis]<node->split){
			left_primitives.push_back(node->primitives[i]);
		}
		else{
			right_primitives.push_back(node->primitives[i]);
		}
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
	split = 0.f;
}

KDnode::~KDnode(){
	// delete (*left);
	// delete(*right);
}

//return node bounding box
//change bbox to this bounding box
BoundingBox* KDnode::getBoundingBox(){

	std::vector<Geometry*>::const_iterator g;//iterator
	g = primitives.begin();

	BoundingBox b = (*primitives.begin())->getBoundingBox();
	for( g = primitives.begin(); g != primitives.end(); ++g ) 
	{
		b = b.join((*g)->getBoundingBox());
	}
	// std::cout << b.min << "bmin1\n";
	// std::cout << b.max << "bmax2\n";
	BoundingBox *bbx = new BoundingBox(b);
	return bbx;
}