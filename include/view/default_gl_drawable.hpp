# pragma once

# include "view/gl_drawable.hpp"

# include "model/game_object.hpp"

# include <iostream>

namespace audiophile
{
  namespace view
  {
    class DefaultGLDrawable : public GLDrawable
    {
      public:

        DefaultGLDrawable( const std::shared_ptr< model::GameObject >& );

        virtual void glDraw( GLRenderer&, GLUTWindow& ) override;

      private:
        std::shared_ptr< model::GameObject > _model;
    };
  }
}
