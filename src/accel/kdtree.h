#include "../scene/scene.h"

using namespace std;

//node
class KDnode{

public:
	KDnode();
	~KDnode();
	BoundingBox bbox;
	KDnode *left; //left child
	KDnode *right; //right child
	
private:
	vector<Geometry*> primitives; //geometry
	int axis;
	float split;
	BoundingBox getBoundingBox();

};

//tree
class KDtree{
public:
	KDtree();
	KDtree(std::vector<Geometry*> prims,int d);
	~KDtree();

	void build(std::vector<Geometry*> prims, int depth, KDnode &node);
	KDnode *root;

	std::vector<Geometry*> primitives;
	int max_depth;
};

