#include <memory>

#include "SDL.h"

#include "view/MainWindow.hpp"

using namespace rise_and_fall;

int main( int, char** ){
	if( SDL_Init(SDL_INIT_EVERYTHING) != 0 ){
		std::cerr << "Fatal Error: Failed to Initialize SDL. " << SDL_GetError() << std::endl;
		return 1;
	}

	auto window = std::make_shared<RF_MainWindow>();

	window->main();

	return 0;
}