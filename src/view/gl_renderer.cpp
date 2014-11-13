# include "view/gl_renderer.hpp"

# include "view/camera.hpp"
# include "view/glut_window.hpp"

# include "GL/freeglut.h"

using namespace std;
using namespace ::view;

GlRenderer::GlRenderer( shared_ptr< model::Game const > const& g )
: _game_model( g ), _camera( new Camera( make_shared< PerspectiveProjection >( .25*M_PI, 0.,100. ) ) )
{}

shared_ptr< ::model::Game const > const& GlRenderer::game_model() const
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
  glClearColor( .0,.0,.0, 1. );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  {
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt( 0,-20,0, 0,0,0, 0,0,1 );
  }

  for( auto o : game_model()->objects() )
  {
    auto drawable = o->getData< Drawable >();
    if( not drawable )
    {
      clog << "::view::GlRenderer::visualize_model: Adding new Drawable for \"" << o->name() << "\"." << endl;
      try{
        drawable = _drawable_factory.create_for( o );
      }catch( out_of_range e )
      {
        //cerr << "::view::GlRenderer::visualize_model: cought exeption: " << e.what() << endl;
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

  // deprecated
  {
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity(); //Reset the camera
    gluPerspective( 45., win.width() / double( win.height() ), .5, 300. );
  }
  _camera->projection()->set_aspect_ratio( win.width() / double( win.height() ) );
}

shared_ptr< Camera > GlRenderer::camera()
{
  return _camera;
}

shared_ptr< Camera const > GlRenderer::camera() const
{
  return _camera;
}