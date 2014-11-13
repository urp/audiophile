# pragma once

# include "view/gl_renderer.hpp"

# include "cg3preview/model/renderbuffer.hpp"

# include <GL/glew.h>

namespace cg3preview
{
  namespace view
  {
    class RenderbufferGlDrawable : public ::view::GlRenderer::Drawable
    {
      public:

        RenderbufferGlDrawable( std::shared_ptr< model::Renderbuffer > const& );

        virtual void visualize( ::view::GlRenderer&, ::view::GlutWindow& ) override;

      private:
        std::shared_ptr< model::Renderbuffer > _renderbuffer;
        GLuint _quad_vbo;
        GLuint _quad_vao;
        GLuint _composed_image_texture;

    }; // RenderbufferGlDrawable

  } // view
} // cg3preview
