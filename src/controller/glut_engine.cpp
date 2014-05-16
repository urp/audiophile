# include "controller/glut_engine.hpp"

using namespace ::controller;

// global file-scope function variable which can be reached from functions GlutEngine::init, GlutEngine::run and glutTimer.
// NOTE: This is ugly(destroys data encapsulation) but unavoidable!?
static std::function< void () > __controller__glut_engine__step_func = [](){ throw std::logic_error( "Default function called in __controller__glut_engine__step_func." ); };

void glutTimer( int interval )
{
  __controller__glut_engine__step_func();
  glutTimerFunc( interval, glutTimer, interval );
}

GlutEngine::GlutEngine( const std::shared_ptr< Logic >& l ) 
: Engine( l, l->game_model() ) 
{}

void GlutEngine::init( int& argc, char** argv )
{
  glutInit( &argc, argv );
  glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION );

  // Set global function which will be invoked by glutTimer.
  __controller__glut_engine__step_func = [this](){ this->step( controller::InputEventHandler::keyboard_event() ); };
  glutTimerFunc( _prefered_timestep_millisec, glutTimer, _prefered_timestep_millisec );
}

void GlutEngine::run()
{
  // Run game.
  glutMainLoop();
}


