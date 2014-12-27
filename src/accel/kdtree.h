#include "../scene/scene.h"

using namespace std;

class KDtree{
	KDtree();
	~KDtree();

	void build(int depth);
	KDnode *root;

	std::vector<Geometry*> primitives;
};

class KDnode{
public:
	KDnode();
	~KDnode();

	void addPrimitive(KDnode *node, Geometry *prim);
	BoundingBox getBoundingBox();
	
	BoundingBox bbox;
	KDnode *left; //left child
	KDnode *right; //right child
	vector<Geometry*> primitives;

private:
	int depth;
	int axis;
	float split;
};