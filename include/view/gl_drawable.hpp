# pragma once

namespace audiophile
{
  namespace view
  {
    class GLRenderer;
    class GLUTWindow;

    class GLDrawable
    {
      public:
        virtual void glDraw( GLRenderer&, GLUTWindow& ) = 0;
    };
  }
}
