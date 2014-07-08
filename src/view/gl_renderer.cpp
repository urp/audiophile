# include "view/gl_renderer.hpp"

# include "view/glut_window.hpp"

# include "GL/freeglut.h"

using namespace ::view;

GlRenderer::GlRenderer( std::shared_ptr< model::Game const > const& g )
: _game_model( g )
{}

std::shared_ptr< ::model::Game const > const& GlRenderer::game_model() const
{
  return _game_model;
}

GlRenderer::delegate_factory_type& GlRenderer::drawable_factory()
{
  return _drawable_factory;
}

GlRenderer::delegate_factory_type const& GlRenderer::drawable_factory() const
{
  return _drawable_factory;
}

void GlRenderer::visualize_model( GlutWindow& w )
{
  glClearColor( .1,.2,.3, 1. );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  gluLookAt( 0,-20,0, 0,0,0, 0,0,1 );

  for( auto o : game_model()->objects() )
  {
    auto drawable = o->getData< Drawable >();
    if( not drawable )
    {
      std::clog << "::view::GlRenderer::visualize_model: Adding new Drawable for \"" << o->name() << "\"." << std::endl;
      try{
        drawable = _drawable_factory.create_for( o );
      }catch( std::out_of_range e )
      {
        std::cerr << "::view::GlRenderer::visualize_model: cought exeption: " << e.what() << std::endl;
        continue;
      }

      o->registerData( drawable );
    }

    if( drawable ) drawable->visualize( *this, w );
  }

  glutSwapBuffers(); 
}

void GlRenderer::resize( GlutWindow& win ) 
{
  glViewport( 0,0, win.width(), win.height() );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity(); //Reset the camera
  gluPerspective( 45., win.width() / double( win.height() ), .5, 300. );
}
