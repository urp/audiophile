# pragma once

# include "controller/input_event_handler.hpp"

# include "model/game.hpp"

namespace audiophile
{
  namespace controller
  {
    class Logic
    {
      public:
        struct ObjectLogic : public model::GameObject::Data
        {
          virtual void advance( Logic& ) = 0;
        };

        Logic() = delete;
        Logic( const std::shared_ptr< model::Game >& );

        virtual void handleKeyboard( const controller::InputEventHandler::keyboard_event& );

        void advance();

        std::shared_ptr< model::Game >       game_model()       { return _model; }
        std::shared_ptr< const model::Game > game_model() const { return _model; } 

      protected:
        std::shared_ptr< model::Game > _model;
        std::shared_ptr< factory_map< model::GameObject, ObjectLogic > > _obj_logic_factory;
    };

  };
}