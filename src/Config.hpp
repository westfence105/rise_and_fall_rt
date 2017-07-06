#pragma once

#ifndef rise_and_fall_Config_h
#define rise_and_fall_Config_h

#include <ios>

#include "boost/filesystem.hpp"

#include "soundbag/SDL_GL_Window.hpp"

namespace rise_and_fall {
  struct Config : soundbag::SDL_GL_Window::Config {
    static boost::filesystem::path path;
    static std::string font;
    static std::string debug;

    Config(int argc, char** argv) throw( std::runtime_error, std::ios_base::failure );
    ~Config();

  private:
    Config(const Config&);
    void init_path();
  };
}

#endif
