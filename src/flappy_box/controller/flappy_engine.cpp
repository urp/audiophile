# include "flappy_box/controller/flappy_engine.hpp"

# include "flappy_box/model/box.hpp"
# include "flappy_box/controller/box_object_logic.hpp"
# include "flappy_box/view/box_gl_drawable.hpp"

# include "view/gl_renderer.hpp"

using namespace ::flappy_box::controller;

static std::function< void () >      __current_glut_advance_func = [](){ std::cerr << "Warning: Default function called in __current_glut_advance_func." << std::endl; };

FlappyEngine::FlappyEngine( const std::shared_ptr< ::controller::Logic >& l )
: ::controller::GlutEngine( l )
, _gl_renderer( std::make_shared< ::view::GlRenderer >( game_model() ) )
{
}


void FlappyEngine::init( int& argc, char** argv )
{
  GlutEngine::init( argc, argv );

  gl_renderer()->drawable_factory().register_module< model::Box >( []( const std::shared_ptr< ::model::GameObject >& p ) { return std::make_shared< view::BoxGlDrawable >( p ); } );

  game_model()->addGameObject( std::make_shared< model::Box >() );
  game_logic()->logic_factory().register_module< model::Box >( []( const std::shared_ptr< ::model::GameObject >& p ) { return std::make_shared< BoxObjectLogic >( p ); } );

}


void FlappyEngine::run()
{
  // Create a window.
  std::shared_ptr< ::view::GlutWindow > window( new ::view::GlutWindow( "flappy_box", 500,500, gl_renderer() , game_logic() ) );

  // run game
  GlutEngine::run();
}


