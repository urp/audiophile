# pragma once

# include "model/game.hpp"
# include "controller/logic.hpp"

namespace controller 
{
  class Engine : public InputEventHandler, public std::enable_shared_from_this< Engine >
  {
    public:
      Engine( const std::shared_ptr< Logic >&
            , const std::shared_ptr< model::Game >& = std::shared_ptr< model::Game >( new model::Game ) 
            );

      virtual void init( int&, char** ) = 0;
      virtual void run() = 0;

      std::shared_ptr< model::Game >       game_model();
      std::shared_ptr< model::Game const > game_model() const;

      std::shared_ptr< Logic >       game_logic();
      std::shared_ptr< Logic const > game_logic() const;

    protected:
      // Calls everything we need to advance the game.
      // Override to integrate new behavior.
      virtual void step( InputEventHandler::keyboard_event const& ev );

      // Implement InputEventHandler. Simply calls function step with the keyboard_event.
      // Input events should be provided by an input-event-provider (e.g. GlutWindow).
      // Override to change event distribution and processing.
      virtual bool handle( ::controller::InputEventHandler::keyboard_event const& ev );

    private:
      std::shared_ptr< model::Game > _model;
      std::shared_ptr< Logic >       _logic;

  }; // Engine

} // controller::
