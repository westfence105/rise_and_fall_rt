#pragma once

#ifndef rise_and_fall_MainWindow_h
#define rise_and_fall_MainWindow_h

#include <mutex>

#include "SDL.h"

#include "soundbag/SDL_GL_Window.hpp"
#include "soundbag/SBGL_Dialog.hpp"

#include "Config.hpp"
#include "Scene.hpp"

namespace rise_and_fall {
	class MainWindow : public soundbag::SDL_GL_Window {
		std::recursive_mutex m_mutex;

		std::shared_ptr<Scene> m_scene;

		std::unique_ptr<soundbag::SBGL_Dialog> m_quit_dialog;

	public:
		MainWindow( const rise_and_fall::Config& conf );
		~MainWindow();

		void setScene( std::shared_ptr<Scene> scene );

	protected:
		void prepare() override;
		void draw() override;
		void update( uint32_t delta ) override;
		bool handleEvent( const SDL_Event& event ) override;

	};
}

#endif
