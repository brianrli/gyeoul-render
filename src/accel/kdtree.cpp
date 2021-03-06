#include "kdtree.h"

//constructor
KDtree::KDtree(std::vector<Geometry*> *prims,int d)
:max_depth(d)
{
	root = new KDnode();
	std::vector<Geometry*> objects = *prims;

	int i = 0;
	std::vector<Geometry*>::iterator g = objects.begin();

	//remove non bounding box geometry objects
	while(g != objects.end()) {
         if(!(*g)->hasBoundingBoxCapability()) {
            g = objects.erase(g);
        }
        else ++g;
	}

	build(objects,max_depth,root);
}


//build tree
void KDtree::build(std::vector<Geometry*> &prims, int& depth, KDnode* node){
	
	//initialize primitives
	node->primitives = prims;
	node->left = new KDnode();
	node->right = new KDnode();
	
	std::vector<Geometry*> left_primitives;
	std::vector<Geometry*> right_primitives;
	node->left->primitives = left_primitives;
	node->right->primitives = right_primitives;
	node->bbox = node->getBoundingBox();

	// std::cout << "KDtree::build\n";
	// std::cout << node->getBoundingBox()->min << "\n";
	// std::cout << node->getBoundingBox()->max << "\n";

	//=====[make leaf]=====
	if(depth == 0){
		return;
	}

	//no primitives
	if(node->primitives.size() == 0){
		return;
	}

	if(node->primitives.size()==1){
		return;
	}

	//=====[ make interior node ]=====

	//choose split position
	node->axis = node->bbox->longest_axis();
	int axis = node->axis;
	// std::cout << "axis is: " << axis << "\n";

	//for now, choose the midpoint of the longest axis
	for(size_t i = 0; i < node->primitives.size(); i++){
		BoundingBox b = node->primitives[i]->getBoundingBox();
		node->split += (b.max[axis]+b.min[axis])/2.0;
	}
	node->split /= node->primitives.size();


	//if left of midpoint
	for(size_t i = 0; i < node->primitives.size(); i++){
		double m = (node->primitives[i]->getBoundingBox().max[axis]+
			node->primitives[i]->getBoundingBox().min[axis])/2.0;
		// std::cout << "m: " << m << "\n";
		if(m<node->split){
			left_primitives.push_back(node->primitives[i]);
		}
		else{
			right_primitives.push_back(node->primitives[i]);
		}
	}

	//detect matches
	if(left_primitives.size() == 0 && right_primitives.size() > 0){
		left_primitives = right_primitives;
	}
	if(right_primitives.size() == 0 && left_primitives.size() > 0){
		right_primitives = left_primitives;
	}

	double matches = 0.5;
	int nmatches = 0;
	for (size_t j = 0; j < left_primitives.size(); j++){
		for (size_t k = 0; k < right_primitives.size(); k++){
			if(left_primitives[j]==right_primitives[k])
				nmatches++;
		}
	}

	if(left_primitives.size()*matches < (double)nmatches || 
		right_primitives.size()*matches < (double)nmatches){
		return;
	}

	// std::cout << "left child " << left_primitives.size() << "matches\n";
	// std::cout << "right child " << right_primitives.size() << "matches\n";
	// std::cout << "\n";

	//recurse
	depth--;
	build(left_primitives, depth,node->left);
	build(right_primitives, depth,node->right);
	return;
}

bool KDtree::intersect(const ray&r, isect&i){
	// std::cout << "tree intersect\n";
	bool have_one = false;
	root->intersect(r,i,have_one);
	// if(have_one){
	// 	std::cout << i.t << "\n";
	// }
	return have_one;
}

bool KDnode::intersect(const ray&r, isect&i, bool&have_one){
	// std::cout << "node intersect " << have_one << " " << primitives.size() << "\n";
	double tMin, tMax;
	isect cur;
	// std::cout << "test bbox\n";
	// std::cout << bbox->min << "\n";
	// std::cout << bbox->max << "\n";
	if(bbox->intersect(r,tMin,tMax)){
		// std::cout << "leggo\n";
		//interior node
		if(left->primitives.size() > 0 || right->primitives.size() > 0 ){
			left->intersect(r,i,have_one);
			right->intersect(r,i,have_one);
		}
		//leaf node
		else{
			for (size_t j = 0; j < primitives.size(); j++){
				if(primitives[j]->intersect(r,cur)){
					if( !have_one || (cur.t < i.t) ) {
						// std:m:cout << cur.t << " " << i.t << "\n";
						i = cur;
						have_one = true;
					}
				}
			}
		}
	}
	// std::cout << "exit\n\n";
	return have_one;
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