# include "controller/logic.hpp"

# include <algorithm>

::controller::Logic::Logic( const std::shared_ptr< ::model::Game >& g ) : _model( g ) {}

void ::controller::Logic::addGameObject( std::shared_ptr< model::GameObject > const& o )
{
  // we do lazy adding in case of inserting an element while iterating on the object vector
  if( not o ) throw std::logic_error( "model::Game::addObject: Invalid object." );
  _waitingObjects.push_back( o );
}

bool ::controller::Logic::advance_model( const ::controller::InputEventHandler::keyboard_event& ev )
{
  game_model()->setTimestamp( std::chrono::steady_clock::now() );

  std::clog << "::controller::Logic::advance: timestep " << std::chrono::duration_cast< std::chrono::milliseconds >( game_model()->timestep() ).count() << '.' << std::endl;

  preprocess( ev );

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

      if( obj_logic ) obj_logic->advance( *this, ev );
    }
  }

  postprocess( ev );

  return false;
}

void controller::Logic::preprocess( controller::InputEventHandler::keyboard_event const& )
{
  removeDeletedGameObjects();
}

void controller::Logic::postprocess( controller::InputEventHandler::keyboard_event const& )
{
  processAddedGameObjects();
}

void ::controller::Logic::processAddedGameObjects()
{
  game_model()->_objects.insert( game_model()->_objects.end(),_waitingObjects.begin(), _waitingObjects.end() );
  _waitingObjects.clear();
}

void ::controller::Logic::removeDeletedGameObjects(  )
{
  auto predicate = []( std::shared_ptr< model::GameObject > const& go )->bool { return go->is_marked_as_deleted(); }; 
  game_model()->_objects.erase( std::remove_if( game_model()->_objects.begin(), game_model()->_objects.end(), predicate )
  , game_model()->_objects.end()
  );
}
