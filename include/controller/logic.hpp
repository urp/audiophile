# pragma once

# include "controller/input_event_handler.hpp"

# include "model/game.hpp"

# include "factory_map.hpp"

namespace controller
{
  class Logic
  {
    public:
      struct ObjectLogic : public model::GameObject::Data
      {
        // Advance Game and handle keyboard_event. Returns true if event has been handled.
        virtual bool advance( Logic&, InputEventHandler::keyboard_event const& ) = 0;
      };

      typedef factory_map< model::GameObject, ObjectLogic > delegate_factory_type;

      Logic() = delete;
      Logic( std::shared_ptr< model::Game > const& );

      // Advance model.
      virtual bool advance_model( controller::InputEventHandler::keyboard_event const& = controller::InputEventHandler::keyboard_event() );

      std::shared_ptr< model::Game >       game_model()       { return _model; }
      std::shared_ptr< model::Game const > game_model() const { return _model; } 

      /// Return factory creating ObjectLogic delegates.
      delegate_factory_type&       logic_factory()       { return _obj_logic_factory; }
      delegate_factory_type const& logic_factory() const { return _obj_logic_factory; }

    protected:
      std::shared_ptr< model::Game > _model;
      delegate_factory_type _obj_logic_factory;

  }; // Logic

}; // controller::