# include "controller/logic.hpp"

# include "controller/default_object_logic.hpp"

audiophile::controller::Logic::Logic( const std::shared_ptr< model::Game >& g )
: _model( g )
, _obj_logic_factory( new factory_map< model::GameObject, ObjectLogic > )
{ }

void audiophile::controller::Logic::advance()
{
  auto last_timestamp = game_model()->timestamp;
  game_model()->timestamp =  std::chrono::steady_clock::now();
  game_model()->timestep  =  game_model()->timestamp - last_timestamp;

  std::clog << "controller::Logic::advance: timestep " << std::chrono::duration_cast< std::chrono::milliseconds >( game_model()->timestep ).count() << '.' << std::endl;

  for( auto o : game_model()->objects() )
  {
    if( o->is_dynamic() )
    {
      auto obj_logic = o->getData< ObjectLogic >();
      if( not obj_logic )
      {
        std::clog << "controller::Logic::advance: Adding new ObjectLogic for \"" << o->name() << "\"." << std::endl;
        o->registerDataType( _obj_logic_factory->create_for( o ) );
        obj_logic = o->getData< ObjectLogic >();
      }

      obj_logic->advance( *this );
    }
  }
}

void audiophile::controller::Logic::handleKeyboard(const audiophile::controller::InputEventHandler::keyboard_event&)
{

}
