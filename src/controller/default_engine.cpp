# include "controller/default_engine.hpp"

# include "model/default_game_object.hpp"

# include "view/default_gl_renderer.hpp"

# include <GL/freeglut.h>

//# include <stdlib.h>
# include <AL/alut.h>
#include <thread>

using namespace audiophile::controller;

DefaultEngine::DefaultEngine()
: Engine( std::make_shared< model::Game >() )
{ }

void DefaultEngine::init( int& argc, char** argv )
{
  alutInit (&argc, argv);
  glutInit( &argc, argv );
  glutIdleFunc( &glutIdle );
  glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS );

  Engine::game_model()->addGameObject( std::make_shared< model::DefaultGameObject >() );
}


void DefaultEngine::run()
{
  std::shared_ptr< view::GLUTWindow > window( new view::GLUTWindow( "audiophile", 500,500, std::make_shared< view::DefaultGLRenderer >( game_model() ), shared_from_this() ) );

  addView( window );

  glutMainLoop();

  alutExit ();
}

void DefaultEngine::glutIdle()
{

}


bool DefaultEngine::handleKeyboardEvent( const InputEventHandler::keyboard_event& ev )
{
  if( ev.key == InputEventHandler::keyboard_event::KEY_A )
  { 
    std::cout << "A pressed" << std::endl;
    {
      ALuint helloBuffer, helloSource;
      helloBuffer = alutCreateBufferHelloWorld ();
      alGenSources (1, &helloSource);
      alSourcei (helloSource, AL_BUFFER, helloBuffer);
      alSourcePlay (helloSource);
      //alutSleep (1);
    }
    std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
    {
      ALuint helloBuffer, helloSource;
      helloBuffer = alutCreateBufferHelloWorld ();
      alGenSources (1, &helloSource);
      alSourcei (helloSource, AL_BUFFER, helloBuffer);
      alSourcePlay (helloSource);
      alutSleep (1);
    }

  }

  return false;
}

