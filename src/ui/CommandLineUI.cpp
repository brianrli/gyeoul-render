#include <iostream>
#include <time.h>
#include <stdarg.h>

#include <assert.h>

#include "CommandLineUI.h"
#include "../fileio/bitmap.h"

#include "../RayTracer.h"
#include "getopt.h"

using namespace std;


// The command line UI simply parses out all the arguments off
// the command line and stores them locally.
CommandLineUI::CommandLineUI( int argc, char** argv )
	: TraceUI()
{
	int i;

	progName=argv[0];
	// std::cout << progName << "\n";

	while( (i = getopt( argc, argv, "tr:w:h:" )) != EOF )
	{
		switch( i )
		{
			case 'r':
				m_nDepth = atoi( optarg );
				break;

			case 'w':
				m_nSize = atoi( optarg );
				break;
			default:
			// Oops; unknown argument
			std::cerr << "Invalid argument: '" << i << "'." << std::endl;
			usage();
			exit(1);
		}

	}

	cout << "out of loop\n";
	// std::cout << optind << " " << argc << "\n";
	if( optind >= argc-1 )
	{
		std::cerr << "no input and/or output name." << std::endl;
		exit(1);
	}

	//old
	// rayName = argv[optind];
	// imgName = argv[optind+1];

	//new
	rayName = argv[optind+1];
	imgName = argv[optind+2];
}

int CommandLineUI::run()
{
	m_nDepth = 3;
	// std::cout << "out\n";
	assert( raytracer != 0 );
	raytracer->loadScene( rayName );
	// raytracer->setDepth(m_nDepth);
	raytracer->setDepth(3);
	std::cout<<"set depth to: " << m_nDepth << "\n";

	if( raytracer->sceneLoaded() )
	{
		int width = m_nSize;
		int height = (int)(width / raytracer->aspectRatio() + 0.5);

		raytracer->traceSetup( width, height );

		clock_t start, end;
		start = clock(); 

		cout << raytracer->aspectRatio();
		for( int j = 0; j < height; ++j )
			for( int i = 0; i < width; ++i ){
				// cout<< "wat\n";
				raytracer->tracePixel(i,j); 
			}

		end=clock();

		// save image
		unsigned char* buf;

		raytracer->getBuffer(buf, width, height);

		if (buf)
			writeBMP(imgName, width, height, buf);

		double t=(double)(end-start)/CLOCKS_PER_SEC;
		std::cout << "total time = " << t << " seconds" << std::endl;
        return 0;
	}
	else
	{
		std::cerr << "Unable to load ray file '" << rayName << "'" << std::endl;
		return( 1 );
	}
}

void CommandLineUI::alert( const string& msg )
{
	std::cerr << msg << std::endl;
}

void CommandLineUI::usage()
{
	std::cerr << "usage: " << progName << " [options] [input.ray output.bmp]" << std::endl;
	std::cerr << "  -r <#>      set recursion level (default " << m_nDepth << ")" << std::endl; 
	std::cerr << "  -w <#>      set output image width (default " << m_nSize << ")" << std::endl;
	std::cerr << "  -a          enable antialiasing" << std::endl;
	std::cerr << "  -A          disable antialiasing (default)" << std::endl;
}
