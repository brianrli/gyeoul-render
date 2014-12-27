#include "../scene/scene.h"

using namespace std;

class KDnode{
public:
	KDnode();
	~KDnode();

	KDnode* build(vector<Geometry*> prims,int depth);
	void addPrimitive(KDnode *node, Geometry *prim);
	BoundingBox getBoundingBox();
	
	KDnode *left; //left child
	KDnode *right; //right child
	vector<Geometry*> primitives;

private:
	int depth;
	int axis;
	float split;
	
};