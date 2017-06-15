cmake_minimum_required(VERSION 3.0)

project(rise_and_fall_rt)

set(CMAKE_CXX_STANDARD 11)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")
endif()

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

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
include_directories(${DEPEND_LIB_ROOT}/include)
link_directories(${DEPEND_LIB_ROOT}/lib)

include_directories(src)

add_executable(rise_and_fall
		src/rise_and_fall.cpp
		src/MainWindow.cpp
		src/Scene.cpp
	)

target_link_libraries(rise_and_fall PUBLIC SDL_GL_Window ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})

install(TARGETS rise_and_fall RUNTIME DESTINATION rise_and_fall)
