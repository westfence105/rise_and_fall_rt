cmake_minimum_required(VERSION 3.0)

project(rise_and_fall_rt)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -DDEBUG")

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(ZLIB REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

set(DEPEND_LIB_ROOT ${CMAKE_CURRENT_BINARY_DIR}/lib)

include(ExternalProject)
ExternalProject_Add(
		libsoundbag
		GIT_REPOSITORY https://github.com/westfence105/libsoundbag.git
		SOURCE_DIR ${DEPEND_LIB_ROOT}/libsoundbag/src
		BINARY_DIR ${DEPEND_LIB_ROOT}/libsoundbag/build
		CMAKE_ARGS -DBUILD_TESTS=OFF -DENABLE_SHARED=OFF
				   -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}
	)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/lib)

include_directories(src)

add_executable(rise_and_fall
		src/rise_and_fall.cpp
		src/MainWindow.cpp
		src/Scene.cpp
		src/Config.cpp
		src/battle/Map.cpp
	)
add_dependencies(rise_and_fall libsoundbag)

target_link_libraries( rise_and_fall PUBLIC
		SDL_GL_Window ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES}
		SDL_ttf
		${Boost_FILESYSTEM_LIBRARIES} ${Boost_SYSTEM_LIBRARIES}
		soundbag_zlib ${ZLIB_LIBRARIES}
	)

install(TARGETS rise_and_fall RUNTIME DESTINATION rise_and_fall)
install(FILES res//NotoSans-Regular.ttf DESTINATION rise_and_fall)
