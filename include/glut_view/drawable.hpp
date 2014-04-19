# pragma once

namespace audiophile
{
  namespace glut_view
  {
    class Renderer;
    class Window;

    class Drawable
    {
      public:
        virtual void glDraw( Renderer&, Window& ) = 0;
      private:
    };
  }
}
