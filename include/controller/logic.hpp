# pragma once

# include "controller/input_event_handler.hpp"

# include "model/game.hpp"

namespace controller
{
  class Logic : public InputEventHandler
  {
    public:
      struct ObjectLogic : public model::GameObject::Data
      {
        // Advance Game and handle keyboard_event. Returns true if event has been handled.
        virtual bool advance( Logic&, const InputEventHandler::keyboard_event& ) = 0;
      };

      typedef factory_map< model::GameObject, ObjectLogic > factory_type;

      Logic() = delete;
      Logic( const std::shared_ptr< model::Game >& );

      virtual bool handle( const controller::InputEventHandler::keyboard_event& );
      virtual bool advance( const controller::InputEventHandler::keyboard_event& = controller::InputEventHandler::keyboard_event() );

      std::shared_ptr< model::Game >       game_model()       { return _model; }
      std::shared_ptr< model::Game const > game_model() const { return _model; } 

      factory_type&       logic_factory()       { return _obj_logic_factory; }
      factory_type const& logic_factory() const { return _obj_logic_factory; }

    protected:
      std::shared_ptr< model::Game > _model;
      factory_type _obj_logic_factory;

  }; // Logic

}; // controller::