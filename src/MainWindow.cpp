#include "GL/gl.h"
#include "SDL.h"

#include "soundbag/Color.hpp"
#include "soundbag/threads.hpp"

#include "MainWindow.hpp"
#include "Config.hpp"

using namespace soundbag;
using namespace rise_and_fall;

MainWindow::MainWindow( const rise_and_fall::Config& conf ) : SDL_GL_Window( "The Rise and Fall", conf )
{
	m_quit_dialog = std::unique_ptr<SBGL_Dialog>( new SBGL_Dialog("終了してよろしいですか？") );
}

MainWindow::~MainWindow(){

}

void MainWindow::prepare(){
	m_quit_dialog->genBuffers();
}

void MainWindow::draw(){
	soundbag::try_lock<std::recursive_mutex> locker(m_mutex);
	if( locker ){
		if( m_scene ){
			if( !(*m_scene) ){
				m_scene->prepare();
			}
			m_scene->draw();
		}
		else {
			glClear(GL_COLOR_BUFFER_BIT);
		}

		if( m_quit_dialog ){
			m_quit_dialog->draw();
		}
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

bool MainWindow::handleEvent( const SDL_Event& event ){
	soundbag::try_lock<std::recursive_mutex> locker(m_mutex);
	if( locker ){
		if( event.type == SDL_QUIT ){
			if( m_quit_dialog ){
				if( m_quit_dialog->isShown() ){
					quit();
				}
				else {
					m_quit_dialog->show();
				}
			}
			else {
				quit();
			}
			return true;
		}
		else if( event.type == SDL_MOUSEBUTTONDOWN ){
			PointF localPos( event.button.x - width() / 2.0, height() / 2.0 - event.button.y );
			if( m_quit_dialog && m_quit_dialog->isShown() ){
				int ret = m_quit_dialog->onClick( localPos );
				if( ret == SBGL_Dialog::RESULT_ACCEPTED ){
					quit();
				}
				else if( ret == SBGL_Dialog::RESULT_REJECTED || ret == SBGL_Dialog::RESULT_CLICKED_BACKGROUND ){
					m_quit_dialog->hide();
				}
				return true;
			}
		}

		if( m_scene ){
			if( m_scene->handleEvent(event) ){
				return true;
			}
		}
	}

	return false;
}

void MainWindow::setScene( std::shared_ptr<Scene> scene ){
	std::unique_lock<std::recursive_mutex> locker(m_mutex);
	m_scene = scene;
}
