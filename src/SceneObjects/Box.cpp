#ifdef _WIN32
namespace std
{
	template <class _Tp>
	inline const _Tp& min(const _Tp& __a, const _Tp& __b) {
	  return __b < __a ? __b : __a;
	}

	template <class _Tp>
	inline const _Tp& max(const _Tp& __a, const _Tp& __b) {
	  return  __a < __b ? __b : __a;
	}
}
#endif


#include <cmath>
#include <assert.h>
#include <algorithm>

#include "Box.h"

using namespace std;

const double HUGE_DOUBLE = 1e100;

bool Box::intersectLocal( const ray& r, isect& i ) const
{
	
	Vec3d p = r.getPosition();
	Vec3d d = r.getDirection();
	double t;
	Vec3d P;

	// std::cout << "[ box intersect ]\n";
	// p.print();
	// d.print();

	double t_hit[3] = {0.0,0.0,0.0};

	//+/-
	double f = 0.5;

	bool hit = true;

	//x direction
	// std::cout<<"[x direction]: ";
	if(p[0]>0.5 || p[0]<-0.5){

		f = (p[0]>0.5) ? 0.5 : -0.5;
		double t = ((f-p[0])/d[0]);
		// std::cout<< f << " " << t << "\n";

		if( t > RAY_EPSILON ) {
			Vec3d P = r.at(t);

			if( P[1] < (-0.5-RAY_EPSILON) || P[1] > (0.5+RAY_EPSILON) ) {	
				hit = false;
			}

			if( P[2] < (-0.5-RAY_EPSILON) || P[2] > (0.5+RAY_EPSILON) ) {	
				hit = false;
			}

			if(hit)
				t_hit[0] = t;
		}
	}
	hit = true;

	// std::cout<<"[y direction]: ";
	//y direction
	if(p[1]>0.5 || p[1]<-0.5){

		f = (p[1]>0.5 ? 0.5 : -0.5);
		double t = ((f-p[1])/d[1]);
		// std::cout<< f << " " << t << "\n";

		if( t > RAY_EPSILON ) {
			Vec3d P = r.at(t);

			if( P[0] < (-0.5-RAY_EPSILON) || P[0] > (0.5+RAY_EPSILON) ) {
				hit = false;
			}

			if( P[2] < (-0.5-RAY_EPSILON) || P[2] > (0.5+RAY_EPSILON) ) {	
				hit = false;
			}

			if(hit){
				t_hit[1] = t;
			}
		}
	}
	hit = true;

	//z direction
	// std::cout<<"[z direction]: ";
	if(p[2]>0.5 || p[2]<-0.5){

		f = (p[2]>0.5 ? 0.5 : -0.5);
		double t = ((f-p[2])/d[2]);
		// std::cout<< f << " " << t << "\n";

		if( t > RAY_EPSILON ) {
			Vec3d P = r.at(t);

			if( P[0] < (-0.5-RAY_EPSILON) || P[0] > (0.5+RAY_EPSILON) ) {	
				hit = false;
			}

			if( P[1] < (-0.5-RAY_EPSILON) || P[1] > (0.5+RAY_EPSILON) ) {	
				hit = false;
			}

			if(hit){
				t_hit[2] = t;
			}
		}
	}

	//find best hit
	t = HUGE_DOUBLE;
	int axis = 0;
	for(int j=0; j<3; j++){
		if((t_hit[j]<t) && (t_hit[j]>RAY_EPSILON)){
			t = t_hit[j];
			axis = j;
		}
	}

	if(t==HUGE_DOUBLE || t==0){
		return false;
	}

	// std::cout << "hit at: ";
	// r.at(t).print();

	Vec3d normal = Vec3d(0.0,0.0,0.0);
	normal[axis] = (d[axis]>0.0) ? -1.0 : 1.0;

	i.obj = this;
	i.t = t;
	i.N = normal;
	
	// std::cout << "[NORMAL IS: ] ";
	// normal.print();

	return true;
}
