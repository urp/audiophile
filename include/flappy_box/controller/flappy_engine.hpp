# pragma once

# include "controller/glut_engine.hpp"

# include <chrono>

namespace flappy_box
{
  namespace controller 
  {

    class FlappyEngine : public ::controller::GlutEngine
    {
      public:
        FlappyEngine( const std::shared_ptr< ::controller::Logic >& );

        virtual void init( int&, char** ) override;
        virtual void run() override;

        std::shared_ptr< ::view::GlRenderer >       gl_renderer()       { return _gl_renderer; }
        std::shared_ptr< ::view::GlRenderer const > gl_renderer() const { return _gl_renderer; }

      private:
        std::shared_ptr< ::view::GlRenderer > _gl_renderer;
    };

  } // controller::
} // flappy_box::