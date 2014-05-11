
# pragma once

# include "model/game.hpp"
# include "controller/input_event_handler.hpp"
# include "controller/logic.hpp"
# include "view/glut_window.hpp"

# include <chrono>

namespace controller 
{
  /// Application class
  class Engine
  {
    public:
      Engine( const std::shared_ptr< Logic >&
            , const std::shared_ptr< model::Game >& = std::shared_ptr< model::Game >( new model::Game ) 
            );

      virtual void init( int&, char** ) = 0;
      virtual void run() = 0;

      std::shared_ptr< model::Game >       game_model();
      std::shared_ptr< const model::Game > game_model() const;

      std::shared_ptr< Logic >       game_logic();
      std::shared_ptr< const Logic > game_logic() const;

    private:
      std::shared_ptr< model::Game > _model;
      std::shared_ptr< Logic >       _logic;
  };

} // controller::
