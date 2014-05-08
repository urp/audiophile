cmake_minimum_required(VERSION 2.8)

project(audiophile)

include_directories( include )

add_definitions( "--std=c++11" )

#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")

#########################################################
# FIND OPENAL
#########################################################
find_package(OpenAL REQUIRED)
include_directories(${OPENAL_INCLUDE_DIRS})
link_directories(${OPENAL_LIBRARY_DIRS})
add_definitions(${OPENAL_DEFINITIONS})
if(NOT OPENAL_FOUND)
message(ERROR " ALUT not found!")
endif(NOT OPENAL_FOUND)

#########################################################
# FIND ALUT
#########################################################
 find_package(ALUT REQUIRED)
 include_directories(${ALUT_INCLUDE_DIRS})
 link_directories(${ALUT_LIBRARY_DIRS})
 add_definitions(${ALUT_DEFINITIONS})
 if(NOT ALUT_FOUND)
 message(ERROR " ALUT not found!")
 endif(NOT ALUT_FOUND)

#########################################################
# EXECUTABLE AUDIOphile
#########################################################

add_executable( audiophile src/main.cpp
                           # model
                           src/model/game_object.cpp
                           # controller
                           src/controller/engine.cpp
                           # view
                           src/view/window.cpp
                           src/view/gl_renderer.cpp
                           # default game
                           src/model/default_game_object.cpp
                           src/controller/default_engine.cpp
                           src/view/default_gl_renderer.cpp
                           src/view/default_gl_drawable.cpp
              )

# create the program "test"
target_link_libraries( audiophile ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${OPENAL_LIBRARY} ${ALUT_LIBRARY} )

install(TARGETS audiophile RUNTIME DESTINATION bin)