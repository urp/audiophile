# pragma once

# include "model/game_object.hpp"

# include <chrono>
# include <vector>
# include <memory>

namespace model
{

  class Game
  {
    public:

      void addGameObject( const std::shared_ptr< GameObject >& o )
      {
        if( not o ) throw std::logic_error( "model::Game::addObject: Invalid object." );
        _objects.push_back( o );
      }
      const std::vector< std::shared_ptr< GameObject > >&  objects() const { return _objects; }

      std::chrono::steady_clock::time_point timestamp;
      std::chrono::steady_clock::duration   timestep;

  private:
    std::vector< std::shared_ptr< GameObject > > _objects;
  }; // Game

} // model::
