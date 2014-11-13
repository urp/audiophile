cmake_minimum_required( VERSION 2.8 )

project(audiophile)

set( CMAKE_BUILD_TYPE "DEBUG" )

include_directories( include )

add_definitions( "--std=c++11" )

# look for Cmake modules in project directory
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")

#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})

#########################################################
# FIND GLEW
#########################################################
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_directories(${GLEW_LIBRARY_DIRS})
add_definitions(${GLEW_DEFINITIONS})

#########################################################
# FIND OPENAL
#########################################################
find_package(OpenAL REQUIRED)
include_directories(${OPENAL_INCLUDE_DIRS})
link_directories(${OPENAL_LIBRARY_DIRS})
add_definitions(${OPENAL_DEFINITIONS})

#########################################################
# FIND ALUT
#########################################################
 find_package(ALUT REQUIRED)
 include_directories(${ALUT_INCLUDE_DIRS})
 link_directories(${ALUT_LIBRARY_DIRS})
 add_definitions(${ALUT_DEFINITIONS})

#########################################################
# LIBRARY audiophile
#########################################################

add_library( audiophile    # model
                           src/model/game.cpp
                           src/model/game_object.cpp
                           # controller
                           src/controller/engine.cpp
                           src/controller/glut_engine.cpp
                           src/controller/logic.cpp
                           # view
                           src/view/gl_renderer.cpp
                           src/view/al_renderer.cpp
                           src/view/projection.cpp
                           src/view/camera.cpp
                           src/view/glut_window.cpp

           )
set( AUDIOPHILE_DEP_LIBRARIES ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARY} ${OPENAL_LIBRARY} ${ALUT_LIBRARY} )

#########################################################
# EXECUTABLE flappy_box
#########################################################

add_executable( flappy_box apps/flappy_box.cpp
                           src/flappy_box/model/box.cpp
                           src/flappy_box/model/tube.cpp
                           src/flappy_box/controller/flappy_engine.cpp 
                           src/flappy_box/controller/box_object_logic.cpp 
                           src/flappy_box/controller/tube_object_logic.cpp 
                           src/flappy_box/view/box_al_audible.cpp
                           src/flappy_box/view/box_gl_drawable.cpp
                           src/flappy_box/view/tube_gl_drawable.cpp
              )

target_link_libraries( flappy_box audiophile ${AUDIOPHILE_DEP_LIBRARIES} )

#########################################################
# EXECUTABLE cg3preview
#########################################################

# find_library( cg3common_LIBS  NAMES cg3common libcg3common )
# find_library( cg3manager_LIBS NAMES cg3manager libcg3manager )
# find_library( cg3pt_LIBS      NAMES cg3pt libcg3pt )

add_definitions( -DUSE_RENDERMANAGER_SIGNALS )

add_executable( cg3preview apps/cg3preview.cpp
                           src/cg3preview/model/renderbuffer.cpp
                           src/cg3preview/controller/engine.cpp 
                           src/cg3preview/controller/renderbuffer_object_logic.cpp 
                           src/cg3preview/view/renderbuffer_gl_drawable.cpp
                           src/cg3preview/view/renderbuffer_al_audible.cpp
              )

target_link_libraries( cg3preview audiophile ${AUDIOPHILE_DEP_LIBRARIES} libcg3common libcg3manager libcg3pt )


install( DIRECTORY include/ DESTINATION include )

install( TARGETS audiophile flappy_box RUNTIME DESTINATION bin 
                                       ARCHIVE DESTINATION lib
       )