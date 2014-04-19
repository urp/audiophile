# include "controller/default_engine.hpp"

# include "model/default_game_object.hpp"

# include "glut_view/default_renderer.hpp"

# include "GL/freeglut.h"

using namespace audiophile::controller;

DefaultEngine::DefaultEngine( const std::shared_ptr< model::Game >& g )
: Engine( g )
{ }

void DefaultEngine::init( int& argc, char** argv )
{
  glutInit( &argc, argv );
  glutIdleFunc( &glutIdle );
  glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS );
}


void DefaultEngine::run()
{
  std::shared_ptr< glut_view::Window > window( new glut_view::Window( 500,500, "audiophile", shared_from_this() ) );
  std::shared_ptr< glut_view::Renderer > renderer( new glut_view::DefaultRenderer( Engine::game_model() ) );
  window->setRenderer( renderer );
  addView( window );

  Engine::game_model()->addGameObject( std::make_shared< model::DefaultGameObject >() );

  glutMainLoop();
}

void DefaultEngine::glutIdle()
{

}

bool DefaultEngine::handleKeyboardEvent( const InputEventHandler::keyboard_event& ev )
{
  if( ev.key == InputEventHandler::keyboard_event::KEY_A ) std::cout << "A pressed" << std::endl; 
  return false;
}

