# pragma once

# include "model/game.hpp"

# include <string>

namespace audiophile
{
  namespace view 
  {
    class GlutWindow;

    class GlRenderer
    {
      public:
        class Drawable
        {
          public:
            virtual void glDraw( GlRenderer&, GlutWindow& ) = 0;
        };

        GlRenderer() = delete;
        GlRenderer( const std::shared_ptr< const model::Game >&, std::string name );

        std::shared_ptr< const model::Game > game_model() const;
        std::string name() const;

        virtual void initialize( GlutWindow& ) = 0;
        virtual void draw( GlutWindow& ) = 0;
        virtual void resize( GlutWindow& ) = 0;
      private:

        std::string _name;
        std::shared_ptr< const model::Game > _game_model;

    }; // Renderer

  } // view::
} // audiophile::
