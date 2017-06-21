#pragma once

#ifndef rise_and_fall_Config_h
#define rise_and_fall_Config_h

#include "boost/filesystem.hpp"

#include "soundbag/SDL_GL_Window.hpp"

namespace rise_and_fall {
  struct Config : soundbag::SDL_GL_Window::Config {
    boost::filesystem::path path;
    boost::filesystem::path font;
    std::string debug;

    static Config& getInstance();
    void init( int argc, char** argv );

  private:
    Config();
    Config(const Config&);
    ~Config();

  };
}

#endif
