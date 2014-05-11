# pragma once

# include "view/gl_renderer.hpp"

# include "model/default_game_object.hpp"

# include <iostream>

namespace audiophile
{
  namespace view
  {
    class DefaultGLDrawable : public GlRenderer::Drawable
    {
      public:

        DefaultGLDrawable( const std::shared_ptr< model::GameObject >& );

        virtual void glDraw( GlRenderer&, GlutWindow& ) override;

      private:
        std::shared_ptr< model::DefaultGameObject > _model;
    };
  }
}
