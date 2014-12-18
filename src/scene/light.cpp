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
    isect i;
	ray shadow_r(P, orientation, ray::SHADOW);
	
	if(debugLights){
		std::cout << "Ray Details:\n";
		P.print();
		orientation.print();
	}

	if (scene->intersect(shadow_r,i)){
		return Vec3d(0,0,0);
	}
	
    return Vec3d(1,1,1);
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

	// You'll need to modify this method to attenuate the intensity 
	// of the light based on the distance between the source and the 
	// point P.  For now, we assume no attenuation and just return 1.0
	return 1.0;
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
	isect i;

	Vec3d direction = position-P;
	direction.normalize();
	ray shadow_r(P, direction, ray::SHADOW);
	std::cout << "Ray Details:\n";
	P.print();
	direction.print();

	if (scene->intersect(shadow_r,i)){
		return Vec3d(0,0,0);
	}

    return Vec3d(1,1,1);
}
