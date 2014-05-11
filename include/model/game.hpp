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

      Game() : _timestamp( std::chrono::steady_clock::now() )
      { }

      void addGameObject( const std::shared_ptr< GameObject >& o )
      {
        if( not o ) throw std::logic_error( "model::Game::addObject: Invalid object." );
        _objects.push_back( o );
      }
      const std::vector< std::shared_ptr< GameObject > >&  objects() const { return _objects; }

      void setTimestamp( std::chrono::steady_clock::time_point const& t )
      {
        auto last_timestamp = timestamp();
        _timestamp = std::chrono::steady_clock::now();
        _timestep = t - last_timestamp;
      }

      std::chrono::steady_clock::time_point const& timestamp() const { return _timestamp; }
      std::chrono::duration< double > const&       timestep() const { return _timestep; }

  private:
    std::vector< std::shared_ptr< GameObject > > _objects;
    std::chrono::steady_clock::time_point        _timestamp;
    std::chrono::duration< double >              _timestep;
  }; // Game

} // model::
