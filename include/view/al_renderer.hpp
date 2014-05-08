# pragma once

# include "model/game.hpp"

# include <string>

namespace audiophile
{
  namespace view 
  {
    class ALRenderer
    {
      public:
        ALRenderer() = delete;
        ALRenderer( const std::shared_ptr< const model::Game >&, std::string name = "ALRenderer" );

        std::shared_ptr< const model::Game > game_model() const;
        std::string name() const;

        virtual void play( ALContext& ) = 0;

      private:

        std::string _name;
        std::shared_ptr< const model::Game > _game_model;

    }; // Renderer

  } // view::
} // audiophile::
