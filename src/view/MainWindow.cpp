#include "GL/gl.h"
#include "SDL.h"

#include "soundbag/Color.hpp"

#include "MainWindow.hpp"

using namespace soundbag;
using namespace rise_and_fall;

RF_MainWindow::RF_MainWindow() : SDL_GL_Window( "The Rise and Fall", SDL_GL_Window::Config( 800, 600 ) )
{
	
}

RF_MainWindow::~RF_MainWindow(){

}

void RF_MainWindow::draw(){
	glClear(GL_COLOR_BUFFER_BIT);
}

void RF_MainWindow::update( uint32_t delta ){

}