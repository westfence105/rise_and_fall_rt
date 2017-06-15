#pragma once

#ifndef rise_and_fall_MainWindow_h
#define rise_and_fall_MainWindow_h

#include "SDL.h"

#include "soundbag/SDL_GL_Window.hpp"

#include "Scene.hpp"

namespace rise_and_fall {
	class RF_MainWindow : public soundbag::SDL_GL_Window {
		std::shared_ptr<Scene> scene;

	public:
		RF_MainWindow();
		~RF_MainWindow();

	protected:
		void draw();
		void update( uint32_t delta );

	};
}

#endif
