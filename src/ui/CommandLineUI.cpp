#include <iostream>
#include <time.h>
#include <stdarg.h>

#include <assert.h>

#include "CommandLineUI.h"
#include "../fileio/bitmap.h"

#include "../RayTracer.h"
// #include "getopt.h"
#include <unistd.h>""

using namespace std;


// The command line UI simply parses out all the arguments off
// the command line and stores them locally.
CommandLineUI::CommandLineUI( int argc, char** argv )
	: TraceUI()
{
	int c;

	progName=argv[0];
	// std::cout << progName << "\n";
	// opterr = 0;

	// for (int i = 0; i < argc ;i++){
	// 	std::cout << argv[i] << " ";
	// }
	// while( (i = getopt( argc, argv, "t:er:w:h:" )) != EOF )
	// {
	// 	switch( i )
	// 	{
	// 		case 'r':
	// 			m_nDepth = atoi( optarg );
	// 			break;

	// 		case 'w':
	// 			std::cout << "case w\n";
	// 			if(optarg == NULL)
	// 				std::cout << "drank\n";
	// 			std::cout << "weeknd\n";
	// 			m_nSize = atoi( optarg );
	// 			std::cout << m_nSize << "\n";
	// 			break;
	// 		default:
	// 		// Oops; unknown argument
	// 		std::cerr << "Invalid argument: '" << i << "'." << std::endl;
	// 		usage();
	// 		exit(1);
	// 	}

	// }

	int aflag = 0;
	int bflag = 0;
	// int c;
	char *cvalue = NULL;
	std::cout << "opt index: " << optind << "\n";
	while ((c = getopt (argc, argv, "w:r:")) != -1){
		switch (c)
			{
			case 'r':
				m_nDepth = atoi(optarg);
				break;
			
			case 'w':
				m_nSize = atoi(optarg);
				break;

			default:
				std::cerr << "Invalid argument: '" << c << "'." << std::endl;
				usage();
				exit(1);
			}
	}
	std::cout << "Depth: " << m_nDepth << " Width: " << m_nSize <<"\n";
	std::cout << "opt index: " << optind << "\n";
	std::cout << argv[optind] << "\n";
	std::cout << "******************************\n";


	if( optind >= argc-1 ){
		std::cerr << "no input and/or output name." << std::endl;
		exit(1);
	}

	//new
	rayName = argv[optind];
	imgName = argv[optind+1];
}

int CommandLineUI::run()
{
	m_nDepth = 3;
	// std::cout << "out\n";
	assert( raytracer != 0 );
	raytracer->loadScene( rayName );
	// raytracer->setDepth(m_nDepth);
	raytracer->setDepth(3);
	// std::cout<<"set depth to: " << m_nDepth << "\n";

	if( raytracer->sceneLoaded() )
	{
		int width = m_nSize;
		int height = (int)(width / raytracer->aspectRatio() + 0.5);

		raytracer->traceSetup( width, height );

		clock_t start, end;
		start = clock(); 

		for( int j = 0; j < height; ++j )
			for( int i = 0; i < width; ++i )
			{
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
