# include "glut_view/default_renderer.hpp"

# include "glut_view/window.hpp"
//# include "glut_view/default_drawable.hpp"


# include "model/default_game_object.hpp"

# include <GL/gl.h>
# include <GL/glu.h>
# include <GL/freeglut.h>

using namespace audiophile::glut_view;

DefaultRenderer::DefaultRenderer( const std::shared_ptr< const model::Game >& g, const std::string& name )
: Renderer( g, name )
{
  _drawable_factory.register_module< model::DefaultGameObject>( []( const std::shared_ptr< model::GameObject >& p ) { return std::make_shared< DefaultDrawable >( p ); } );
}

void DefaultRenderer::glInitialize( Window& win )
{
  glResize( win );
  glDraw( win );
}

void DefaultRenderer::glDraw( Window& w )
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
  glLoadIdentity(); //Reset the drawing perspective

  for( auto obj : this->game_model()->objects() )
    _drawable_factory.create_for( obj )->glDraw( *this, w );

  glutSwapBuffers(); //Send the 3D scene to the screen
}

void DefaultRenderer::glResize( Window& win ) 
{
  glViewport( 0,0, win.width(), win.height() );
  glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
  glLoadIdentity(); //Reset the camera
  gluPerspective( 45., win.width() / double( win.height() ), .5, 100. );
}
