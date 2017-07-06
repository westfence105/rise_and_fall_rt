#include "GL/gl.h"
#include "SDL.h"

#include "soundbag/Color.hpp"
#include "soundbag/threads.hpp"

#include "MainWindow.hpp"
#include "Config.hpp"

using namespace soundbag;
using namespace rise_and_fall;

MainWindow::MainWindow() : SDL_GL_Window( "The Rise and Fall", rise_and_fall::Config::getInstance() )
MainWindow::MainWindow( const rise_and_fall::Config& conf ) : SDL_GL_Window( "The Rise and Fall", conf )
{

}

MainWindow::~MainWindow(){

}

void MainWindow::draw(){
	soundbag::try_lock<std::recursive_mutex> locker(m_mutex);
	if( locker && m_scene ){
		if( !(*m_scene) ){
			m_scene->prepare();
		}
		m_scene->draw();
	}
	else {
		glClear(GL_COLOR_BUFFER_BIT);
	}
}

void MainWindow::update( uint32_t delta ){
	soundbag::try_lock<std::recursive_mutex> locker(m_mutex);
	if( locker && m_scene ){
		m_scene->update(delta);
		auto next = m_scene->getNext();
		if( next ){
			m_scene = next;
		}
	}
}

bool MainWindow::handleEvent( SDL_Event& event ){
bool MainWindow::handleEvent( const SDL_Event& event ){
	soundbag::try_lock<std::recursive_mutex> locker(m_mutex);
	return ( locker && m_scene && m_scene->handleEvent(event) );
}

void MainWindow::setScene( std::shared_ptr<Scene> scene ){
	std::unique_lock<std::recursive_mutex> locker(m_mutex);
	m_scene = scene;
}
