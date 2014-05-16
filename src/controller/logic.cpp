# include "controller/logic.hpp"

using namespace ::controller;

Logic::Logic( const std::shared_ptr< ::model::Game >& g ) : _model( g ) {}

bool Logic::advance_model( const ::controller::InputEventHandler::keyboard_event& ev )
{
  game_model()->setTimestamp( std::chrono::steady_clock::now() );

  std::clog << "::controller::Logic::advance: timestep " << std::chrono::duration_cast< std::chrono::milliseconds >( game_model()->timestep() ).count() << '.' << std::endl;

  for( auto o : game_model()->objects() )
  {
    if( o->is_dynamic() )
    {
      auto obj_logic = o->getData< ObjectLogic >();
      if( not obj_logic )
      {
        std::clog << "::controller::Logic::advance: Adding new ObjectLogic for \"" << o->name() << "\"." << std::endl;
        obj_logic = _obj_logic_factory.create_for( o );
        o->registerData( obj_logic );
      }

      if( obj_logic and obj_logic->advance( *this, ev ) ) return true;
    }
  }

  return false;
}
