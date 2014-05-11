# include "view/gl_renderer.hpp"

# include "view/glut_window.hpp"

# include "GL/freeglut.h"

using namespace ::view;

GlRenderer::GlRenderer( const std::shared_ptr< const model::Game >& g, const std::string n )
: _game_model( g )
, _name( n )
{}

std::shared_ptr< const ::model::Game > GlRenderer::game_model() const
{
  return _game_model;
}


std::string GlRenderer::name() const
{
  return _name;
}

void GlRenderer::initialize( GlutWindow& win )
{
  resize( win );
  draw( win );
}

void GlRenderer::draw( GlutWindow& w )
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
  glLoadIdentity(); //Reset the drawing perspective

  for( auto obj : this->game_model()->objects() )
    _drawable_factory.create_for( obj )->glDraw( *this, w );

  glutSwapBuffers(); //Send the 3D scene to the screen
}

void GlRenderer::resize( GlutWindow& win ) 
{
  glViewport( 0,0, win.width(), win.height() );
  glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
  glLoadIdentity(); //Reset the camera
  gluPerspective( 45., win.width() / double( win.height() ), .5, 100. );
}
