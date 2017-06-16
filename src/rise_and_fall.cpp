#include <fstream>
#include <memory>

#include "SDL.h"
#include "SDL_ttf.h"

#include "MainWindow.hpp"
#include "RF_Config.hpp"

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

	if( SDL_Init(SDL_INIT_EVERYTHING) != 0 ){
		std::cerr << "Fatal Error: Failed to Initialize SDL. " << SDL_GetError() << std::endl;
		return 1;
	}

	TTF_Init();

	RF_Config::getInstance().init(argc,argv);

	auto window = std::make_shared<RF_MainWindow>();

	window->main();

	TTF_Quit();
	SDL_Quit();

	#ifdef STDOUT_REDIRECT
		std::cout.rdbuf( ob_orig );
		std::cerr.rdbuf( eb_orig );
	#endif

	return 0;
}
