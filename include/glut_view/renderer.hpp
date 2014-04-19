# pragma once

# include "model/game.hpp"

# include <string>

namespace audiophile
{
  namespace glut_view 
  {
    class Window;

    class Renderer
    {
      public:
        Renderer() = delete;
        Renderer( const std::shared_ptr< const model::Game >&, std::string name );

        std::shared_ptr< const model::Game > game_model() const;
        std::string name() const;

        virtual void glInitialize( Window& ) = 0;
        virtual void glDraw( Window& ) = 0;
        virtual void glResize( Window& ) = 0;
      private:

        std::string _name;
        std::shared_ptr< const model::Game > _game_model;

    }; // Renderer

  } // glut_view::
} // audiophile::
