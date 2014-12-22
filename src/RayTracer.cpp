// The main ray tracer.
#include "RayTracer.h"
#include "scene/light.h"
#include "scene/material.h"
#include "scene/ray.h"

#include "parser/Tokenizer.h"
#include "parser/Parser.h"

#include "ui/TraceUI.h"
#include <cmath>
#include <algorithm>

extern TraceUI* traceUI;

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h> 

using namespace std;

// Use this variable to decide if you want to print out
// debugging messages.  Gets set in the "trace single ray" mode
// in TraceGLWindow, for example.
bool debugMode = false;

// Trace a top-level ray through normalized window coordinates (x,y)
// through the projection plane, and out into the scene.  All we do is
// enter the main ray-tracing method, getting things started by plugging
// in an initial ray weight of (0.0,0.0,0.0) and an initial recursion depth of 0.
Vec3d RayTracer::trace( double x, double y )
{
	// Clear out the ray cache in the scene for debugging purposes,
	scene->intersectCache.clear();

    ray r( Vec3d(0,0,0), Vec3d(0,0,0), ray::VISIBILITY );

    scene->getCamera().rayThrough( x,y,r );
	Vec3d ret = traceRay( r, Vec3d(1.0,1.0,1.0), max_depth);
	ret.clamp();
	return ret;
}

bool refract(Vec3d &D, Vec3d &N, double index, Vec3d &t){
	//index = n/n_t
	double a = 1-(pow(index,2)*(1-pow(D*N,2)));
	
	if(a<0.0){
		return false;
	}

	t = index*(D-(N*(D*N))) - N*(sqrt(a));
	t.normalize();
	
	return true;
}

// Do recursive ray tracing!  You'll want to insert a lot of code here
// (or places called from here) to handle reflection, refraction, etc etc.
Vec3d RayTracer::traceRay( const ray& r, 
	const Vec3d& thresh, int depth )
{
	// std::cout << "max depth: " << max_depth << "\n";
	int refl_depth = depth;
	int refr_depth = depth;
	
	isect i;
	Vec3d I;

	if(scene->intersect( r, i )){
		// YOUR CODE HERE

		// An intersection occured!  We've got work to do.  For now,
		// this code gets the material for the surface that was intersected,
		// and asks that material to provide a color for the ray.  

		// This is a great place to insert code for recursive ray tracing.
		// Instead of just returning the result of shade(), add some
		// more steps: add in the contributions from reflected and refracted
		// rays.

		const Material& m = i.getMaterial();
		I = m.shade(scene, r, i);
		
		//=====[ Reflection ]=====
		if(refl_depth!=0){
			Vec3d d = r.getDirection();
			
			//new direction
			Vec3d refl = d - (2*(d*i.N)*i.N);
			refl.normalize();

			if(debugMode){
				std::cout << "reflect direction ";
				refl.print();
			}
			//reflectance ray
			ray refl_ray(r.at(i.t), refl, ray::REFLECTION);
			refl_depth--;
			Vec3d refl_contribution = traceRay(refl_ray,thresh,refl_depth);
			I = I + prod(m.kr(i),refl_contribution);

			//debugging
			if(debugMode){
				std::cout << "kr: ";
				m.kr(i).print();
				std::cout << "contribution: ";
				refl_contribution.print();
				std::cout << "depth: " << depth << "\n";
			}
		}

		//=====[ Refraction ]=====
		if(refr_depth!=0){
			//test if dielectric
			if(m.index(i) > 1.0 || r.type()==ray::REFRACTION){

				//misc
				bool total_int = false;

				Vec3d N = i.N;
				Vec3d d = r.getDirection();
				double n= m.index(i); //interior index (nt)
				Vec3d a = m.kt(i);

				//new vector declarations
				Vec3d t = Vec3d(0,0,0);

				// entering material
				if(d*N < 0){
					if(refract(d,N,1/n,t)){
						ray refr_ray(r.at(i.t),t,ray::REFRACTION);
						refr_depth -= 1;
						Vec3d refraction = traceRay(refr_ray,thresh,refr_depth);
						I += prod(m.kt(i),refraction);

					}
				}
				// leaving material
				else{
					Vec3d b = -N;
					if(refract(d,b,n,t)){
						ray refr_ray(r.at(i.t),t,ray::REFRACTION);
						refr_depth -= 1;
						Vec3d refraction = traceRay(refr_ray,thresh,refr_depth);
						I += prod(m.kt(i),refraction);
					}
				}
			}
		}


		//=====[ return radiance ]=====
		return I;
	} else {
		// No intersection.  This ray travels to infinity, so we color
		// it according to the background color, which in this (simple) case
		// is just black.

		return Vec3d( 0.0, 0.0, 0.0 );
	}
}

RayTracer::RayTracer()
	: max_depth(0), scene( 0 ), buffer( 0 ), buffer_width( 256 ), buffer_height( 256 ), m_bBufferReady( false )
{
}


RayTracer::~RayTracer()
{
	delete scene;
	delete [] buffer;
}

void RayTracer::getBuffer( unsigned char *&buf, int &w, int &h )
{
	buf = buffer;
	w = buffer_width;
	h = buffer_height;
}

double RayTracer::aspectRatio()
{
	return sceneLoaded() ? scene->getCamera().getAspectRatio() : 1;
}

bool RayTracer::loadScene( char* fn )
{
	ifstream ifs( fn );
	if( !ifs ) {
		string msg( "Error: couldn't read scene file " );
		msg.append( fn );
		traceUI->alert( msg );
		return false;
	}
	
	// Strip off filename, leaving only the path:
	string path( fn );
	if( path.find_last_of( "\\/" ) == string::npos )
		path = ".";
	else
		path = path.substr(0, path.find_last_of( "\\/" ));

	// Call this with 'true' for debug output from the tokenizer
	Tokenizer tokenizer( ifs, false );
    Parser parser( tokenizer, path );
	try {
		delete scene;
		scene = 0;
		scene = parser.parseScene();
	} 
	catch( SyntaxErrorException& pe ) {
		traceUI->alert( pe.formattedMessage() );
		return false;
	}
	catch( ParserException& pe ) {
		string msg( "Parser: fatal exception " );
		msg.append( pe.message() );
		traceUI->alert( msg );
		return false;
	}
	catch( TextureMapException e ) {
		string msg( "Texture mapping exception: " );
		msg.append( e.message() );
		traceUI->alert( msg );
		return false;
	}


	if( ! sceneLoaded() )
		return false;

	return true;
}

void RayTracer::traceSetup( int w, int h )
{
	if( buffer_width != w || buffer_height != h )
	{
		buffer_width = w;
		buffer_height = h;

		bufferSize = buffer_width * buffer_height * 3;
		delete [] buffer;
		buffer = new unsigned char[ bufferSize ];

	}
	memset( buffer, 0, w*h*3 );
	m_bBufferReady = true;
}

void RayTracer::tracePixel( int i, int j )
{
	Vec3d col;

	if( ! sceneLoaded() )
		return;

	double x = double(i)/double(buffer_width);
	double y = double(j)/double(buffer_height);

	col = trace( x,y );
	unsigned char *pixel = buffer + ( i + j * buffer_width ) * 3;

	pixel[0] = (int)( 255.0 * col[0]);
	pixel[1] = (int)( 255.0 * col[1]);
	pixel[2] = (int)( 255.0 * col[2]);
}

