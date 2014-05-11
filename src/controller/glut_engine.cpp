# include "controller/glut_engine.hpp"

# include "controller/logic.hpp"

# include "model/default_game_object.hpp"

# include "view/default_gl_renderer.hpp"

# include <AL/alut.h>
# include <GL/freeglut.h>

using namespace audiophile::controller;

static std::function< void () >      __current_glut_advance_func = [](){ std::cerr << "Warning: Default function called in __current_glut_advance_func." << std::endl; };
static std::function< void ( int ) > __current_glut_keyboard_func = []( int ){ std::cerr << "Warning: Default function called in __current_glut_keyboard_func." << std::endl; };


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

  __current_glut_advance_func = std::bind( &Logic::advance, game_logic() );

  glutTimerFunc( 50, glutTimer, 20 );

  Engine::game_model()->addGameObject( std::make_shared< model::DefaultGameObject >() );
}


void GlutEngine::run()
{
  std::shared_ptr< view::GlutWindow > window( new view::GlutWindow( "audiophile", 500,500, std::make_shared< view::DefaultGlRenderer >( game_model() ), game_logic() ) );

  glutMainLoop();

  alutExit();
}


