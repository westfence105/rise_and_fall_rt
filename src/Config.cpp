#include <fstream>
#include <unordered_map>

extern "C" {
  #include <stdlib.h>
  #ifdef _WIN32
  #include <windows.h>
  #endif
  #ifdef __unix
  #include <unistd.h>
  #endif
  #ifdef __APPLE__
  #include <mach-o/dyld.h>
  #endif
  #ifndef MAX_PATH
  #define MAX_PATH _MAX_PATH
  #endif
}

#include "GL/glew.h"

#include "boost/filesystem.hpp"

#include "soundbag/SBGL_Dialog.hpp"

#include "Config.hpp"

using namespace std::string_literals;

namespace bfs = boost::filesystem;

using namespace soundbag;
using namespace rise_and_fall;

boost::filesystem::path Config::path;
std::string Config::font;
std::string Config::debug;

Config::Config( int argc, char** argv )
      throw( std::runtime_error, std::ios_base::failure )
  : SDL_GL_Window::Config( 800, 600 )
{
  uint32_t bufsize = MAX_PATH + 1;
  std::vector<char> buf(bufsize);
  #if defined(_WIN32) || defined(_WIN64)
    GetModuleFileName( NULL, buf.data(), buf.size()-1 );
  #elif TARGET_OS_MAC
    _NSGetExecutablePath(buf.data(),&bufsize);
  #elif defined(__linux)
    readlink("/proc/self/exe", buf.data(), buf.size()-1 );
  #elif defined(__sun)
    const char* execname = getexecname();
    std::vector<char>( execname, std::end(execname) ).swap(buf)
  #elif defined(__NetBSD__) || defined(__DragonFly__)
    readlink("/proc/curproc/exe", buf.data(), sizeof(buf)-1 );
  #elif defined(__FreeBSD__)
    int mib[] = {
      CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1
    };
    sysctl( mib, 4, buf, &bufsize, NULL, 0 );
  #endif
  path = bfs::path(buf.data()).remove_filename();

  font = (path / "TakaoPGothic.ttf").string();

#ifndef NDEBUG
  std::cout << "buf:  " << buf.data() << std::endl;
  std::cout << "path: " << path << std::endl;
  std::cout << "conf: " << path / "rise_and_fall.conf" << std::endl;
  std::cout << "font: " << font << std::endl;
#endif

  std::unordered_map<std::string,std::string> conf;
  try {
    bfs::ifstream conf_ifs( path / "rise_and_fall.conf" );
    if( conf_ifs.fail() ){
      #ifndef NDEBUG
        std::cout << path / "rise_and_fall.conf" << std::endl;
      #endif
      throw 1;
    }
    std::string line;
    while( std::getline( conf_ifs, line ) ){
      const auto dlm_pos = line.find_first_of('=');
      if( dlm_pos != std::string::npos && 0 < dlm_pos && dlm_pos + 1 < line.length() ){
        std::string key = line.substr( 0, dlm_pos );
        std::string value = line.substr( dlm_pos + 1, std::string::npos );
        conf[key] = value;
      #ifndef NDEBUG
        std::cout << key << ":\t" << value << std::endl;
      #endif
      }
      else {
        #ifndef NDEBUG
          std::cerr << "Illegal line: " << line << std::endl;
        #endif
      }
    }
  }
  catch(...){

  }

  for( int i = 1; i < argc; ++i ){
    std::string arg( argv[i] );
    if( ( arg == "--window-mode" || arg == "-w" ) && ++i < argc ){
      conf["window_mode"] = argv[i];
    }
    else if( ( arg == "--window-size" || arg == "-s" ) && ++i < argc  ){
      conf["window_size"] = argv[i];
    }
    else if( ( arg == "--window-pos" || arg == "-p" ) && ++i < argc ){
      conf["window_pos"] = argv[i];
    }
  #ifndef NDEBUG
    else if( arg == "--debug" && ++i < argc ){
      debug = argv[i];
    }
  #endif
    else {
      std::cerr << "Warning: Ignoring invalid argument \"" << arg << "\"." << std::endl;
    }
  }

  auto find = [&]( const std::string& key, const std::string& _default ){
    auto iter = conf.find(key);
    if( iter != conf.end() ){
      return iter->second;
    }
    else {
      return _default;
    }
  };

  std::string wm_str = find("window_mode","normal");
  std::cout << wm_str << std::endl;
  if( wm_str == "full" || wm_str == "fullscreen" ){
    window_mode = WINDOW_MODE_FULLSCREEN;
  }
  else if( wm_str == "max" || wm_str == "maximized" ){
    window_mode = WINDOW_MODE_MAXIMIZED;
  }
  else {
    window_mode = WINDOW_MODE_NORMAL;
  }

  std::string ws_str = find("window_size","");
  const auto ws_x_pos = ws_str.find_first_of('x');
  if( ws_x_pos != std::string::npos ){
    try {
      const auto width_str  = ws_str.substr( 0, ws_x_pos );
      std::cout << width_str << std::endl;
      width = std::stoi( width_str );
    } catch(...){}
    try {
      const auto height_str = ws_str.substr( ws_x_pos + 1, std::string::npos );
      height = std::stoi( height_str );
    } catch(...){}
  }

  if( SDL_Init(SDL_INIT_EVERYTHING) != 0 ){
    throw std::runtime_error("Failed to initialize SDL. "s + SDL_GetError() );
  }

  TTF_Init();

  SBGL_Dialog::msg_font = TTF_OpenFont( font.c_str(), 24 );
  SBGL_Dialog::button_font = TTF_OpenFont( font.c_str(), 22 );
  if( SBGL_Dialog::msg_font == NULL || SBGL_Dialog::button_font == NULL ){
    throw std::ios_base::failure("Failed to load font \""s + font + "\".");
  }
}

Config::~Config(){
  TTF_CloseFont( SBGL_Dialog::msg_font );
  TTF_CloseFont( SBGL_Dialog::button_font );

  TTF_Quit();
  SDL_Quit();
}