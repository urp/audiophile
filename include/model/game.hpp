# pragma once

# include "model/player.hpp"
# include "model/game_object.hpp"

# include <vector>
# include <memory>

namespace audiophile
{
  namespace model
  {

    class Game
    {
      public:

        
        void addPlayer( const std::shared_ptr< Player > p ) 
        {
          if( not p ) throw std::logic_error( "model::Game::addObject: Invalid object." );
          _player = p;
        }
        std::shared_ptr< const Player > player() const { return _player; }

        void addGameObject( const std::shared_ptr< GameObject >& o )
        {
          if( not o ) throw std::logic_error( "model::Game::addObject: Invalid object." );
          _objects.push_back( o );
        }
        const std::vector< std::shared_ptr< GameObject > >&  objects() const { return _objects; }

      private:
        std::shared_ptr< Player >                    _player;
        std::vector< std::shared_ptr< GameObject > > _objects;
    }; // Game

  } // model::
} // audiophile::