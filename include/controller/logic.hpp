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
        /// Advance Game and handle keyboard_event. Returns true if event has been handled.
        virtual bool advance( Logic&, InputEventHandler::keyboard_event const& ) = 0;
      };

      typedef factory_map< model::GameObject, ObjectLogic > delegate_factory_type;

      Logic() = delete;
      Logic( std::shared_ptr< model::Game > const& );

      void addGameObject( std::shared_ptr< model::GameObject > const& o );

      /// Advance model.
      virtual bool advance_model( controller::InputEventHandler::keyboard_event const& = controller::InputEventHandler::keyboard_event() );

      std::shared_ptr< model::Game >       game_model()       { return _model; }
      std::shared_ptr< model::Game const > game_model() const { return _model; } 

      /// Return factory creating ObjectLogic delegates.
      delegate_factory_type&       logic_factory()       { return _obj_logic_factory; }
      /// Return factory creating ObjectLogic delegates.
      delegate_factory_type const& logic_factory() const { return _obj_logic_factory; }

    protected:
      /// Overwrite to do custom processing before any object logic is executed (called in advance_model).
      virtual void preprocess( controller::InputEventHandler::keyboard_event const& );
      /// Overwrite to do custom postprocessing after all object logic was executed (called in advance_model).
      virtual void postprocess( controller::InputEventHandler::keyboard_event const& );

      void processAddedGameObjects();
      void removeDeletedGameObjects();

      std::shared_ptr< model::Game > _model;
      /// List of objects to be added at the end of advance_model function.
      std::vector< std::shared_ptr< model::GameObject > > _waitingObjects;
      delegate_factory_type _obj_logic_factory;

  }; // Logic

}; // controller::