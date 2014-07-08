# pragma once

# include "model/game_object.hpp"

# include <chrono>
# include <vector>
# include <memory>

namespace controller
{
  class Logic;
}

namespace model
{
  class Game
  {
      friend class ::controller::Logic;
    public:

      Game();

      void addGameObject( std::shared_ptr< GameObject > const& o );

      const std::vector< std::shared_ptr< GameObject > >& objects() const;

      void setTimestamp( std::chrono::steady_clock::time_point const& t );

      std::chrono::steady_clock::time_point const& timestamp() const;
      std::chrono::duration< double > const&       timestep() const;
      std::chrono::duration< double > const&       runtime() const;

    private:
      std::vector< std::shared_ptr< GameObject > > _objects;
      std::chrono::steady_clock::time_point        _timestamp;
      std::chrono::duration< double >              _timestep;
      std::chrono::duration< double >              _runtime;

  }; // Game

} // model::
