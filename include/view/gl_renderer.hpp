# pragma once

# include "model/game.hpp"

# include <string>

namespace audiophile
{
  namespace view 
  {
    class GLUTWindow;

    class GLRenderer
    {
      public:
        class Drawable
        {
          public:
            virtual void glDraw( GLRenderer&, GLUTWindow& ) = 0;
        };

        GLRenderer() = delete;
        GLRenderer( const std::shared_ptr< const model::Game >&, std::string name );

        std::shared_ptr< const model::Game > game_model() const;
        std::string name() const;

        virtual void initialize( GLUTWindow& ) = 0;
        virtual void draw( GLUTWindow& ) = 0;
        virtual void resize( GLUTWindow& ) = 0;
      private:

        std::string _name;
        std::shared_ptr< const model::Game > _game_model;

    }; // Renderer

  } // view::
} // audiophile::
