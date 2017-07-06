#include <fstream>
#include <memory>

#include "GL/glew.h"
#include "SDL.h"
#include "SDL_ttf.h"

#include "MainWindow.hpp"
#include "Config.hpp"

using namespace std::string_literals;

using namespace rise_and_fall;

int main( int argc, char** argv ){
	#ifdef _WIN32
	#define STDOUT_REDIRECT
		auto ob_orig = std::cout.rdbuf();
		auto eb_orig = std::cerr.rdbuf();
	#ifndef NDEBUG
		std::ofstream ofs_out("stdout.txt");
		std::ofstream ofs_err("stderr.txt");
	#else
		std::ofstream ofs_out("rise_and_fall_out.log.txt");
		std::ofstream ofs_err("rise_and_fall_err.log.txt");
	#endif
		std::cout.rdbuf( ofs_out.rdbuf() );
		std::cerr.rdbuf( ofs_err.rdbuf() );
	#endif

	try{
		Config conf( argc, argv );

		auto window = std::make_shared<MainWindow>( conf );

		auto err = glewInit();
		if( err != GLEW_OK ){
			throw std::runtime_error( "Failed to initializing GLEW. "s +
										std::string((const char*)glewGetErrorString(err))
								);
		}

		window->main();
	}
	catch( const std::exception &e ){
		std::cerr << "Fatal Error: " << e.what() << std::endl;
		return 1;
	}

	#ifdef STDOUT_REDIRECT
		std::cout.rdbuf( ob_orig );
		std::cerr.rdbuf( eb_orig );
	#endif

	return 0;
}
