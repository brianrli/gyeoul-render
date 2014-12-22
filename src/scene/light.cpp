#include <cmath>

#include "light.h"

using namespace std;
bool debugLights = false;

double DirectionalLight::distanceAttenuation( const Vec3d& P ) const
{
	// distance to light is infinite, so f(di) goes to 0.  Return 1.
	return 1.0;
}


Vec3d DirectionalLight::shadowAttenuation( const Vec3d& P ) const
{
    // YOUR CODE HERE:
	// should return a color
	isect i;
	Vec3d shadow = Vec3d(1,1,1);
	Vec3d direction = -orientation;

	ray shadow_r(P, direction, ray::SHADOW);
	
	//std::cout << "Ray Details:\n";
	//P.print();
	//direction.print();
	bool cont = true;
	bool in = false;

	while (scene->intersect(shadow_r,i)){
		const Material& m = i.getMaterial();
		if(!in){
			shadow = prod(m.kt(i),shadow);
			in = true;
		}
		else
			in = false;
	
		shadow_r = ray(shadow_r.at(i.t),
		direction,ray::SHADOW);		
	}

	// std::cout<<"Shadow Contritution: ";
	// shadow.print();
    return shadow;
}

Vec3d DirectionalLight::getColor( const Vec3d& P ) const
{
	// Color doesn't depend on P 
	return color;
}

Vec3d DirectionalLight::getDirection( const Vec3d& P ) const
{
	return -orientation;
}

double PointLight::distanceAttenuation( const Vec3d& P ) const
{
	// YOUR CODE HERE
	double d = (position - P).length();
	double il = min(1.0,1/(constantTerm+(linearTerm * d)+(quadraticTerm*pow(d,2))));

	// min(1,1/( a + b d + c d^2 ))
	return il;
}

Vec3d PointLight::getColor( const Vec3d& P ) const
{
	// Color doesn't depend on P 
	return color;
}

Vec3d PointLight::getDirection( const Vec3d& P ) const
{
	Vec3d ret = position - P;
	ret.normalize();
	return ret;
}


Vec3d PointLight::shadowAttenuation(const Vec3d& P) const
{
    // YOUR CODE HERE:
	// should return a color
	isect i;
	Vec3d shadow = Vec3d(1,1,1);

	Vec3d direction = position-P;
	direction.normalize();
	ray shadow_r(P, direction, ray::SHADOW);
	
	//std::cout << "Ray Details:\n";
	//P.print();
	//direction.print();
	bool cont = true;
	bool in = false;

	while (scene->intersect(shadow_r,i)){
		const Material& m = i.getMaterial();
		if(!in){
			shadow = prod(m.kt(i),shadow);
			in = true;
		}
		else
			in = false;
	
		shadow_r = ray(shadow_r.at(i.t),
		direction,ray::SHADOW);		
	}

	// std::cout<<"Shadow Contritution: ";
	// shadow.print();
    return shadow;
}
