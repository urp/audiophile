# pragma once

# include "glut_view/renderer.hpp"
# include "glut_view/default_drawable.hpp"

# include "factory.hpp"

namespace audiophile
{
  namespace glut_view 
  {

    class DefaultRenderer : public Renderer
    {
      public:

        DefaultRenderer( const std::shared_ptr< const model::Game >&, const std::string& = "DefaultRenderer" );

        virtual void glInitialize( Window& ) override;
        virtual void glDraw( Window& ) override;
        virtual void glResize( Window& ) override;

      protected:
        void setGlViewport() const;

      private:
        factory_map< model::GameObject, DefaultDrawable > _drawable_factory;

    }; // Renderer

  } // glut_view::
} // audiophile::
