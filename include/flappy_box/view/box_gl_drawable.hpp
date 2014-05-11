# pragma once

# include "view/gl_renderer.hpp"

# include "flappy_box/model/box.hpp"

# include <iostream>

namespace flappy_box
{
  namespace view
  {
    class BoxGlDrawable : public ::view::GlRenderer::Drawable
    {
      public:

        BoxGlDrawable( const std::shared_ptr< model::Box >& );

        virtual void visualize( ::view::GlRenderer&, ::view::GlutWindow& ) override;

      private:
        std::shared_ptr< model::Box > _model;
    };
  }
}
