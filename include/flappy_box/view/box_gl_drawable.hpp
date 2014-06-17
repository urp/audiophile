# pragma once

# include "view/gl_renderer.hpp"

# include "flappy_box/model/box.hpp"

namespace flappy_box
{
  namespace view
  {
    class BoxGlDrawable : public ::view::GlRenderer::Drawable
    {
      public:

        BoxGlDrawable( std::shared_ptr< model::Box > const& );

        virtual void visualize( ::view::GlRenderer&, ::view::GlutWindow& ) override;

      private:
        std::shared_ptr< model::Box const > _box;

    }; // BoxGlDrawable
  }
}
