# pragma once

# include "controller/glut_engine.hpp"

# include <cg3/manager/render_manager.hpp>

# include "view/al_renderer.hpp"
# include "view/gl_renderer.hpp"

namespace cg3preview
{
  namespace controller 
  {

    class Engine : public ::controller::GlutEngine
    {
      public:
        Engine( std::shared_ptr< ::controller::Logic > const& = std::make_shared< ::controller::Logic >( std::make_shared< ::model::Game >() ) );

        virtual void init( int& argc, char** argv ) override;
        virtual void run() override;

        std::shared_ptr< ::view::AlRenderer >       al_renderer()       { return _al_renderer; }
        std::shared_ptr< ::view::AlRenderer const > al_renderer() const { return _al_renderer; }

        std::shared_ptr< ::view::GlRenderer >       gl_renderer()       { return _gl_renderer; }
        std::shared_ptr< ::view::GlRenderer const > gl_renderer() const { return _gl_renderer; }

      protected:
        virtual void step( InputEventHandler::keyboard_event const& ) override;

      private:
        std::shared_ptr< RenderManager > _render_manager;

        std::shared_ptr< ::view::AlRenderer > _al_renderer;
        std::shared_ptr< ::view::GlRenderer > _gl_renderer;

        std::shared_ptr< ::view::GlutWindow > _main_window;

    }; // Engine

  } // controller::
} // cg3preview::