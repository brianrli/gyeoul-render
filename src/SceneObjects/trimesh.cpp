#include <cmath>
#include <float.h>
#include "trimesh.h"

using namespace std;
 
Trimesh::~Trimesh()
{
	for( Materials::iterator i = materials.begin(); i != materials.end(); ++i )
		delete *i;
}

// must add vertices, normals, and materials IN ORDER
void Trimesh::addVertex( const Vec3d &v )
{
    vertices.push_back( v );
}

void Trimesh::addMaterial( Material *m )
{
    materials.push_back( m );
}

void Trimesh::addNormal( const Vec3d &n )
{
    normals.push_back( n );
}

// Returns false if the vertices a,b,c don't all exist
bool Trimesh::addFace( int a, int b, int c )
{
    int vcnt = vertices.size();

    if( a >= vcnt || b >= vcnt || c >= vcnt )
        return false;

    TrimeshFace *newFace = new TrimeshFace( scene, new Material(*this->material), this, a, b, c );
    newFace->setTransform(this->transform);
    faces.push_back( newFace );
    scene->add(newFace);
    return true;
}

char *
Trimesh::doubleCheck()
// Check to make sure that if we have per-vertex materials or normals
// they are the right number.
{
    if( !materials.empty() && materials.size() != vertices.size() )
        return "Bad Trimesh: Wrong number of materials.";
    if( !normals.empty() && normals.size() != vertices.size() )
        return "Bad Trimesh: Wrong number of normals.";

    return 0;
}

// Calculates and returns the normal of the triangle too.
bool TrimeshFace::intersectLocal( const ray& r, isect& it ) const
{    
    // YOUR CODE HERE:
    int X = 0;
    int Y = 1;
    int Z = 2;

    //variable declarations
    Vec3d re = r.getPosition();
    Vec3d rd = r.getDirection();

    // std::cout << "ray position: " <<re[0] << " " << re[1] << " " << re[2] << "\n";
    // std::cout << "ray direction: " <<rd[0] << " " << rd[1] << " " << rd[2] << "\n";
    // std::cout << "ray secondpoint: " <<re[0]+rd[0] << " " << re[1]+rd[1] << " " << re[2]+rd[2] << "\n";


    Vec3d va = parent->vertices[ids[0]];
    Vec3d vb = parent->vertices[ids[1]];
    Vec3d vc = parent->vertices[ids[2]];



    // std::cout << va << " " << vb << " " << vc << "\n";
    Vec3d normal = (va-vb)^(va-vc);
    // std::cout << normal[0] << " " << normal[1] << " " << normal[2] << "\n";

    double a = va[X]-vb[X];
    double b = va[Y]-vb[Y];
    double c = va[Z]-vb[Z];

    double d = va[X]-vc[X];
    double e = va[Y]-vc[Y];
    double f = va[Z]-vc[Z];

    double g = rd[X];
    double h = rd[Y];
    double i = rd[Z];

    double j = va[X]-re[X];
    double k = va[Y]-re[Y];
    double l = va[Z]-re[Z];

    double M = a*(e*i - h*f) + b*(g*f - d*i) + c*(d*h - e*g);
    
    //compute T
    double t;
    t = -(f*(a*k-j*b)+e*(j*c-a*l)+d*(b*l-k*c))/M;
    if(t < RAY_EPSILON){
        // std:: cout << t << "t failed\n\n";
        return false;
    }
        
    //compute gamma
    double gamma;
    gamma = (i*(a*k - j*b) + h*(j*c-a*l) + g*(b*l-k*c))/M;
    if((gamma < 0) || (gamma>1)){
        // std::cout << gamma << " gamma failed " << M <<"\n";
        return false;
    }
    // else
    //     std:: cout<<gamma<<"\n";

    //compute beta
    double beta;
    beta = (j*(e*i - h*f) + k*(g*f - d*i) + l*(d*h - e*g))/M;
    if((beta < 0) || (beta > 1-gamma)){
        // std::cout << beta << " beta failed" << 1-gamma << "\n";
        return false;
    }

    //=======[ Phong Normal Interpolation ]======

    // Vec3d na = parent->vertices[ids[0]];
    // Vec3d nb = parent->vertices[ids[1]];
    // Vec3d nc = parent->vertices[ids[2]];
    if(parent->materials.size() != 0){
        Vec3d P = r.at(t);
        Vec3d na = (vc-vb)^(P-vb);
        Vec3d nb = (va-vc)^(P-vc);
        Vec3d nc = (vb-va)^(P-va);

        //normal not necessarily of unit length
        double n2 = pow(normal.length(),2);
        double Sigma = (normal*na)/n2;
        double Beta = (normal*nb)/n2;
        double Gamma = (normal*nc)/n2;        

        Material ca = *parent->materials[ids[0]];
        Material cb = *parent->materials[ids[1]];
        Material cc = *parent->materials[ids[2]]; 

        Material c_out;
        ca = Sigma * ca;
        cb = Beta * cb;
        cc = Gamma * cc;
        
        c_out += ca;
        c_out += cb;
        c_out += cc;

        // std::cout<< "BSG: " << Beta << " " << Sigma << " " << Gamma << "\n";
        // std::cout << "normal is" << normal << "\n";
        // std::cout << "n2: " << n2 << "\n";
        // std::cout << na << " " << normal*na << "\n";
        // std::cout << nb << " " << normal*nb << "\n";
        // std::cout << nc << " " << normal*nc << "\n";

        it.setMaterial(c_out);
    } 

    // Add triangle intersection code here.
    normal.normalize();
    it.setN(normal);
    it.setT(t);
    it.obj = this;

	// std::cout << "tmesh isect\n"; 
    return true;
}

void
Trimesh::generateNormals()
// Once you've loaded all the verts and faces, we can generate per
// vertex normals by averaging the normals of the neighboring faces.
{
    int cnt = vertices.size();
    normals.resize( cnt );
    int *numFaces = new int[ cnt ]; // the number of faces assoc. with each vertex
    memset( numFaces, 0, sizeof(int)*cnt );
    
    for( Faces::iterator fi = faces.begin(); fi != faces.end(); ++fi )
    {
        Vec3d a = vertices[(**fi)[0]];
        Vec3d b = vertices[(**fi)[1]];
        Vec3d c = vertices[(**fi)[2]];
        
        Vec3d faceNormal = ((b-a) ^ (c-a));
		faceNormal.normalize();
        
        for( int i = 0; i < 3; ++i )
        {
            normals[(**fi)[i]] += faceNormal;
            ++numFaces[(**fi)[i]];
        }
    }

    for( int i = 0; i < cnt; ++i )
    {
        if( numFaces[i] )
            normals[i]  /= numFaces[i];
    }

    delete [] numFaces;
}

