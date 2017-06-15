#include <memory>

#include "SDL.h"
#include "SDL_ttf.h"

#include "MainWindow.hpp"
#include "RF_Config.hpp"

using namespace rise_and_fall;

int main( int argc, char** argv ){
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

	return 0;
}
