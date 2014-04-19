project(audiophile)

include_directories( include )

add_definitions( "--std=c++11" )

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
# EXECUTABLE AUDIOphile
#########################################################

add_executable( audiophile src/main.cpp
                           # model
                           src/model/game_object.cpp
                           # controller
                           src/controller/engine.cpp
                           # view
                           src/glut_view/window.cpp
                           src/glut_view/renderer.cpp
                           # default game
                           src/model/default_game_object.cpp
                           src/controller/default_engine.cpp
                           src/glut_view/default_renderer.cpp
                           src/glut_view/default_drawable.cpp
              )

# create the program "test"
target_link_libraries( audiophile ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

install(TARGETS audiophile RUNTIME DESTINATION bin)
