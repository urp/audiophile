# pragma once

# include "view/gl_renderer.hpp"
# include "view/default_gl_drawable.hpp"

# include "factory.hpp"

namespace audiophile
{
  namespace view 
  {

    class DefaultGLRenderer : public GLRenderer
    {
      public:

        DefaultGLRenderer( const std::shared_ptr< const model::Game >&, const std::string& = "DefaultRenderer" );

        virtual void initialize( GLUTWindow& ) override;
        virtual void draw( GLUTWindow& ) override;
        virtual void resize( GLUTWindow& ) override;

      protected:
        void setGlViewport() const;

      private:
        factory_map< model::GameObject, DefaultGLDrawable > _drawable_factory;

    }; // Renderer

  } // view::
} // audiophile::
