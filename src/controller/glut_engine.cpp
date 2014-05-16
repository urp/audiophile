# include "controller/glut_engine.hpp"

using namespace ::controller;

// global file-scope function variable which can be reached from GlutEngine::init, GlutEngine::run and glutTimera
// NOTE: This is ugly but unavoidable!?
static std::function< void () > __controller_glut_engine_step_func = [](){ std::cerr << "Warning: Default function called in __current_glut_advance_func." << std::endl; };

GlutEngine::GlutEngine( const std::shared_ptr< Logic >& l ): Engine( l, l->game_model() )
{
}

void glutTimer( int interval )
{
  __controller_glut_engine_step_func();
  glutTimerFunc( interval, glutTimer, interval );
}

void GlutEngine::init( int& argc, char** argv )
{
  glutInit( &argc, argv );

  glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION );

  // Set global function which will be invoked by glutTimer.
  __controller_glut_engine_step_func = [this](){ this->step( controller::InputEventHandler::keyboard_event() ); };
  glutTimerFunc( _prefered_timestep_millisec, glutTimer, _prefered_timestep_millisec );
}


void GlutEngine::run()
{
  // run game
  glutMainLoop();

}


