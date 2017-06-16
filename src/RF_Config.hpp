#pragma once

#ifndef rise_and_fall_RF_Config_h
#define rise_and_fall_RF_Config_h

#include "boost/filesystem.hpp"

#include "soundbag/SDL_GL_Window.hpp"

namespace rise_and_fall {
  struct RF_Config : soundbag::SDL_GL_Window::Config {
    boost::filesystem::path path;
    boost::filesystem::path font;
    std::string debug;

    static RF_Config& getInstance();
    void init( int argc, char** argv );

  private:
    RF_Config();
    RF_Config(const RF_Config&);
    ~RF_Config();

  };
}

#endif
