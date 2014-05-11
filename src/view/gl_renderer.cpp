# include "view/gl_renderer.hpp"

# include "view/glut_window.hpp"

# include "GL/freeglut.h"

using namespace ::view;

GlRenderer::GlRenderer( const std::shared_ptr< const model::Game >& g )
: _game_model( g )
{}

std::shared_ptr< const ::model::Game > GlRenderer::game_model() const
{
  return _game_model;
}

void GlRenderer::initialize( GlutWindow& win )
{
  resize( win );
  draw( win );
}

void GlRenderer::draw( GlutWindow& w )
{
  glClearColor( 0.1, 0.2, 0.3, 1.);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt( 0,-7,0, 0,0,0, 0,0,1 );

  for( auto o : game_model()->objects() )
  {
    auto drawable = o->getData< Drawable >();
    if( not drawable )
    {
      std::clog << "::view::GlRenderer::draw: Adding new Drawable for \"" << o->name() << "\"." << std::endl;
      o->registerDataType( _drawable_factory.create_for( o ) );
      drawable = o->getData< Drawable >();
    }

    drawable->visualize( *this, w );
  }

  glutSwapBuffers(); 
}

void GlRenderer::resize( GlutWindow& win ) 
{
  glViewport( 0,0, win.width(), win.height() );
  glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
  glLoadIdentity(); //Reset the camera
  gluPerspective( 45., win.width() / double( win.height() ), .5, 100. );
}
