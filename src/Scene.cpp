#include <future>
#include <mutex>

#include "soundbag/threads.hpp"

#include "Scene.hpp"

using namespace rise_and_fall;

Scene::Scene(){

}

Scene::~Scene(){

}

std::shared_ptr<Scene> Scene::getNext(){
  soundbag::try_lock<std::recursive_mutex> locker( m_mutex );
  if( locker && next_scene ){
    return next_scene;
  }
  else {
    return NULL;
  }
}
