# include "flappy_box/controller/flappy_engine.hpp"

# include "flappy_box/model/box.hpp"
# include "flappy_box/model/tube.hpp"

# include "flappy_box/controller/box_object_logic.hpp"
# include "flappy_box/controller/tube_object_logic.hpp"
# include "flappy_box/view/box_gl_drawable.hpp"
# include "flappy_box/view/tube_gl_drawable.hpp"
# include "flappy_box/view/box_al_audible.hpp"

# include "view/glut_window.hpp"
# include "view/camera.hpp"

# include <AL/alut.h>
# include <GL/freeglut.h>

using namespace std;
using namespace ::flappy_box::controller;

FlappyEngine::FlappyEngine( const shared_ptr< ::controller::Logic >& l )
: ::controller::GlutEngine( l )
, _al_renderer( make_shared< ::view::AlRenderer >( game_model() ) )
, _gl_renderer( make_shared< ::view::GlRenderer >( game_model() ) )
{}

void FlappyEngine::init( int& argc, char** argv )
{
  GlutEngine::init( argc, argv );

  alutInit( &argc, argv );

  auto tube = make_shared< model::Tube >();
  auto box = make_shared< model::Box  >( tube );
  tube->setBox( box );

  game_model()->addGameObject( tube );
  game_model()->addGameObject( box );

  game_logic() ->   logic_factory().register_derived_key< model::Box  >( []( shared_ptr< model::Box  > const& b ) { return make_shared<  BoxObjectLogic >     ( b ); } );
  game_logic() ->   logic_factory().register_derived_key< model::Tube >( []( shared_ptr< model::Tube > const& t ) { return make_shared< TubeObjectLogic >     ( t ); } );

  gl_renderer()->drawable_factory().register_derived_key< model::Box  >( []( shared_ptr< model::Box  > const& b ) { return make_shared< view:: BoxGlDrawable >( b ); } );
  gl_renderer()->drawable_factory().register_derived_key< model::Tube >( []( shared_ptr< model::Tube > const& t ) { return make_shared< view::TubeGlDrawable >( t ); } );

  al_renderer()-> audible_factory().register_derived_key< model::Box >( []( shared_ptr< model::Box > const& b ) { return make_shared< view::BoxAlAudible > ( b ); } );
}


void FlappyEngine::run()
{
  // Create a window and connect it with a view::GlRenderer and an InputEventHandler.
  size_t const win_width  = 500;
  size_t const win_height = 500;
  _main_window = make_shared< ::view::GlutWindow >( "flappy_box", win_width,win_height, gl_renderer() , shared_from_this() );

  gl_renderer()->camera()->lookAt( vec3_type( 0,-20,0 ), vec3_type( 0,0,0 ), vec3_type( 0,0,1 ) );
  gl_renderer()->camera()->set_projection( make_shared< ::view::PerspectiveProjection >( .25*M_PI, 0.,100. , double( win_width )/ win_height ) );

  // run game
  GlutEngine::run();

  alutExit();
}

void FlappyEngine::step( ::controller::InputEventHandler::keyboard_event const& ev )
{
  ::controller::Engine::step( ev );
  al_renderer()->auralize_model();
  _main_window->invalidate();
}

