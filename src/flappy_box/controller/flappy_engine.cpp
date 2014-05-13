# include "flappy_box/controller/flappy_engine.hpp"

# include "flappy_box/model/box.hpp"
# include "flappy_box/controller/box_object_logic.hpp"
# include "flappy_box/view/box_gl_drawable.hpp"
# include "flappy_box/view/box_al_audible.hpp"

# include "view/glut_window.hpp"

using namespace ::flappy_box::controller;

static std::function< void () >      __current_glut_advance_func = [](){ std::cerr << "Warning: Default function called in __current_glut_advance_func." << std::endl; };

FlappyEngine::FlappyEngine( const std::shared_ptr< ::controller::Logic >& l )
: ::controller::GlutEngine( l )
, _gl_renderer( std::make_shared< ::view::GlRenderer >( game_model() ) )
, _al_renderer( std::make_shared< ::view::AlRenderer >( game_model() ) )
{
}

void FlappyEngine::init( int& argc, char** argv )
{
  GlutEngine::init( argc, argv );

  game_model()->addGameObject( std::make_shared< model::Box >() );

  game_logic()->logic_factory().register_module< model::Box >( []( std::shared_ptr< model::Box > const& b ) { return std::make_shared< BoxObjectLogic >( b ); } );
  gl_renderer()->drawable_factory().register_module< model::Box >( []( std::shared_ptr< model::Box > const& b ) { return std::make_shared< view::BoxGlDrawable >( b ); } );
  al_renderer()->audible_factory().register_module< model::Box >( []( std::shared_ptr< model::Box > const& b ) { return std::make_shared< view::BoxAlAudible >( b ); } );
}


void FlappyEngine::run()
{
  // Create a window.
  std::shared_ptr< ::view::GlutWindow > window( new ::view::GlutWindow( "flappy_box", 500,500, gl_renderer() , shared_from_this() ) );

  // run game
  GlutEngine::run();
}

void FlappyEngine::step( ::controller::InputEventHandler::keyboard_event const& ev )
{
  ::controller::Engine::step( ev );
  al_renderer()->auralize_model();
}

