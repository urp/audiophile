# pragma once

# include "glut_view/drawable.hpp"

# include "model/game_object.hpp"

# include <iostream>

namespace audiophile
{
  namespace glut_view
  {
    class DefaultDrawable : public Drawable
    {
      public:

        DefaultDrawable( const std::shared_ptr< model::GameObject >& );

        virtual void glDraw( Renderer&, Window& ) override;

      private:
        std::shared_ptr< model::GameObject > _model;
    };
  }
}
