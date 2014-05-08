# include "view/default_gl_renderer.hpp"

# include "view/glut_window.hpp"

# include "model/default_game_object.hpp"

# include <GL/gl.h>
# include <GL/glu.h>
# include <GL/freeglut.h>

using namespace audiophile::view;

DefaultGLRenderer::DefaultGLRenderer( const std::shared_ptr< const model::Game >& g, const std::string& name )
: GLRenderer( g, name )
{
  _drawable_factory.register_module< model::DefaultGameObject>( []( const std::shared_ptr< model::GameObject >& p ) { return std::make_shared< DefaultGLDrawable >( p ); } );
}

void DefaultGLRenderer::initialize( GLUTWindow& win )
{
  resize( win );
  draw( win );
}

void DefaultGLRenderer::draw( GLUTWindow& w )
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
  glLoadIdentity(); //Reset the drawing perspective

  for( auto obj : this->game_model()->objects() )
    _drawable_factory.create_for( obj )->glDraw( *this, w );

  glutSwapBuffers(); //Send the 3D scene to the screen
}

void DefaultGLRenderer::resize( GLUTWindow& win ) 
{
  glViewport( 0,0, win.width(), win.height() );
  glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
  glLoadIdentity(); //Reset the camera
  gluPerspective( 45., win.width() / double( win.height() ), .5, 100. );
}
