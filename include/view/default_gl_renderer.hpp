# pragma once

# include "view/gl_renderer.hpp"
# include "view/default_gl_drawable.hpp"

# include "factory_map.hpp"

namespace audiophile
{
  namespace view 
  {

    class DefaultGlRenderer : public GlRenderer
    {
      public:

        DefaultGlRenderer( const std::shared_ptr< const model::Game >&, const std::string& = "DefaultRenderer" );

        virtual void initialize( GlutWindow& ) override;
        virtual void draw( GlutWindow& ) override;
        virtual void resize( GlutWindow& ) override;

      protected:
        void setGlViewport() const;

      private:
        factory_map< model::GameObject, DefaultGLDrawable > _drawable_factory;

    }; // Renderer

  } // view::
} // audiophile::
