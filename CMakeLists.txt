cmake_minimum_required(VERSION 3.0)

project(rise_and_fall_rt)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -DDEBUG")

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Boost COMPONENTS system filesystem python REQUIRED)
find_package(PythonLibs 2 REQUIRED)

include_directories(
		${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} 
	 	${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS}
	)

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

add_library(battle STATIC
		src/battle/Unit.cpp
		src/battle/Unit_draw.cpp
		src/battle/Unit_logic.cpp
	)

add_library(rise_and_fall_pyd SHARED src/python/rise_and_fall.cpp)
set_target_properties(rise_and_fall_pyd PROPERTIES 
		OUTPUT_NAME rise_and_fall
		SUFFIX ".pyd"
	)

target_link_libraries( rise_and_fall_pyd PRIVATE 
		battle ${Boost_PYTHON_LIBRARIES} ${PYTHON_LIBRARIES}
	)

target_link_libraries( rise_and_fall PUBLIC
		battle rise_and_fall_pyd
		SDL_GL_Window ${SDL2_LIBRARIES} SDL2_ttf
		${GLEW_LIBRARIES} ${OPENGL_LIBRARIES}
		${Boost_PYTHON_LIBRARIES} ${PYTHON_LIBRARIES}
		${Boost_FILESYSTEM_LIBRARIES} ${Boost_SYSTEM_LIBRARIES}
		soundbag_zlib ${ZLIB_LIBRARIES}
	)

install(TARGETS rise_and_fall 
			RUNTIME DESTINATION rise_and_fall
			LIBRARY DESTINATION rise_and_fall/script
	)
install(FILES res//NotoSans-Regular.ttf DESTINATION rise_and_fall)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
add_executable(python_test test/python_test.cpp)
target_link_libraries(python_test 
	battle rise_and_fall_pyd
	${Boost_PYTHON_LIBRARIES} ${PYTHON_LIBRARIES}
	${GTEST_BOTH_LIBRARIES})
add_test(NAME python_test COMMAND $<TARGET_FILE:python_test>)