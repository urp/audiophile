# include "controller/glut_engine.hpp"

# include "controller/logic.hpp"

# include <AL/alut.h>
# include <GL/freeglut.h>

using namespace ::controller;

static std::function< void () > __current_glut_advance_func = [](){ std::cerr << "Warning: Default function called in __current_glut_advance_func." << std::endl; };

GlutEngine::GlutEngine( const std::shared_ptr< Logic >& l ): Engine( l, l->game_model() )
{
}

void glutTimer( int interval )
{
  __current_glut_advance_func();
  glutTimerFunc( interval, glutTimer, interval );
  glutPostRedisplay();
}

void GlutEngine::init( int& argc, char** argv )
{
  alutInit( &argc, argv );

  glutInit( &argc, argv );

  glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION );

  __current_glut_advance_func = std::bind( &Logic::advance, game_logic(), controller::InputEventHandler::keyboard_event() );

  glutTimerFunc( _prefered_timestep_millisec, glutTimer, _prefered_timestep_millisec );
}


void GlutEngine::run()
{
  // run game
  glutMainLoop();

  alutExit();
}


