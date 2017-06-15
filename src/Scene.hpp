#pragma once

#ifndef rise_and_fall_Scene_h
#define rise_and_fall_Scene_h

#include <future>
#include <memory>
#include <mutex>

#include "SDL.h"

namespace rise_and_fall {
  class Scene {
    std::shared_ptr<Scene> next_scene;
    std::recursive_mutex m_mutex;

  protected:
    template<class T,class... Args>
    void loadNext( Args... args ){
      std::thread(
        [&]{
          std::unique_lock<std::recursive_mutex> locker(m_mutex);
          next_scene = std::make_shared<T>(args...);
        }
      ).detach();
    }

    bool prepared;

    virtual void genBuffers() = 0;

  public:
    Scene();
    virtual ~Scene();

    virtual void prepare();
    virtual void draw() const = 0;
    virtual void update( uint32_t delta ) = 0;
    virtual bool handleEvent( const SDL_Event& event ) = 0;
    std::shared_ptr<Scene> getNext();

    operator bool() const {
      return prepared;
    }

  };
}

#endif
