# include "cg3preview/controller/engine.hpp"

# include "cg3preview/model/renderbuffer.hpp"

# include "cg3preview/controller/renderbuffer_object_logic.hpp"
# include "cg3preview/view/renderbuffer_gl_drawable.hpp"
# include "cg3preview/view/renderbuffer_al_audible.hpp"

# include "view/glut_window.hpp"
# include "view/camera.hpp"

# include <cg3/pathtracer/pathtracer.hpp>

# include <cg3/manager/progressive_strategy.hpp>

# include <cg3/common/test_scenes.hpp>
# include <cg3/common/image_io.hpp>

# include <AL/alut.h>
# include <GL/freeglut.h>

using namespace ::cg3preview;
using namespace ::cg3preview::controller;

Engine::Engine( const shared_ptr< ::controller::Logic >& l )
: ::controller::GlutEngine( l )
, _al_renderer( make_shared< ::view::AlRenderer >( game_model() ) )
, _gl_renderer( make_shared< ::view::GlRenderer >( game_model() ) )
{}

void Engine::init( int& argc, char** argv )
{
  GlutEngine::init( argc, argv );

  alutInit( &argc, argv );

  // initialize _render_manager
  { tiny_vec< size_t, 2 > resolution{ size_t(512), size_t(512) };

    auto my_scene = create_pt_test_scene( resolution );

    shared_ptr< Pathtracer > tracer( new Pathtracer( my_scene ) );

    shared_ptr< Strategy > tiling( new ProgressiveStrategy( { size_t(4), size_t(4) }, 256 ) );

    _render_manager = std::make_shared< RenderManager >( resolution, tracer, tiling, 7 );

    shared_ptr< model::Renderbuffer > rb( new model::Renderbuffer( _render_manager ) );

    game_model()->addGameObject( rb );
  }

  // add delegates interpreting game objects
  game_logic() ->   logic_factory().register_derived_key< model::Renderbuffer >( []( shared_ptr< model::Renderbuffer > const& b ) { return make_shared< controller::RenderbufferObjectLogic >( b ); } );
  gl_renderer()->drawable_factory().register_derived_key< model::Renderbuffer >( []( shared_ptr< model::Renderbuffer > const& b ) { return make_shared< view::RenderbufferGlDrawable >( b ); } );
  al_renderer()-> audible_factory().register_derived_key< model::Renderbuffer >( []( shared_ptr< model::Renderbuffer > const& b ) { return make_shared< view::RenderbufferAlAudible > ( b ); } );
}


void Engine::run()
{
  // Create a window and connect it with a view::GlRenderer and an InputEventHandler.
  size_t const win_width  = _render_manager->get_resolution()[0];
  size_t const win_height = _render_manager->get_resolution()[1];
  _main_window = make_shared< ::view::GlutWindow >( "cg3preview", win_width,win_height, gl_renderer() , shared_from_this() );

  // NOTE: Camera has default identity view matrix
  // Setup a othographic projection matrix.
  gl_renderer()->camera()->set_projection( make_shared< ::view::OrthographicProjection >( 2., 2., 0.,100., double( win_width )/ win_height ) );

  // Run game
  GlutEngine::run();

  // Write 
  write_bmp( _render_manager->get_image(), "image.bmp" );

  alutExit();
}

void Engine::step( ::controller::InputEventHandler::keyboard_event const& ev )
{
  ::controller::Engine::step( ev );
  al_renderer()->auralize_model();
  _main_window->invalidate();
}
