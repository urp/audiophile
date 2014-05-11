# include "controller/logic.hpp"

using namespace ::controller;

Logic::Logic( const std::shared_ptr< ::model::Game >& g )
: _model( g )
{ }

bool Logic::handle( const InputEventHandler::keyboard_event& ev )
{
  return advance( ev );
}

bool Logic::advance( const ::controller::InputEventHandler::keyboard_event& ev )
{
  auto last_timestamp = game_model()->timestamp;
  game_model()->timestamp =  std::chrono::steady_clock::now();
  game_model()->timestep  =  game_model()->timestamp - last_timestamp;

  std::clog << "::controller::Logic::advance: timestep " << std::chrono::duration_cast< std::chrono::milliseconds >( game_model()->timestep ).count() << '.' << std::endl;

  for( auto o : game_model()->objects() )
  {
    if( o->is_dynamic() )
    {
      auto obj_logic = o->getData< ObjectLogic >();
      if( not obj_logic )
      {
        std::clog << "::controller::Logic::advance: Adding new ObjectLogic for \"" << o->name() << "\"." << std::endl;
        o->registerDataType( _obj_logic_factory.create_for( o ) );
        obj_logic = o->getData< ObjectLogic >();
      }

      if( obj_logic->advance( *this, ev ) ) return true;
    }
  }
}
