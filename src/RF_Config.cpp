#include <fstream>
#include <map>

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

#include "boost/filesystem.hpp"

#include "RF_Config.hpp"

using namespace soundbag;
using namespace rise_and_fall;

RF_Config::RF_Config() : SDL_GL_Window::Config( 800, 600 ){
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
  path = boost::filesystem::path(buf.data()).remove_filename();

#if DEBUG
  std::ofstream ofs("stdout.txt");
  ofs << "buf:  " << buf.data() << std::endl;
  ofs << "path: " << path << std::endl;
  ofs << "conf: " << path.append("rise_and_fall.conf") << std::endl;
#endif
}

RF_Config::~RF_Config(){

}

RF_Config& RF_Config::getInstance(){
  static RF_Config instance;
  return instance;
}

void RF_Config::init( int argc, char** argv ) {
  std::vector<std::string> args;
  args.reserve(argc);
  for( int i = 0; i < argc; ++i ){
    args.push_back( std::string(argv[i]) );
  }
}
