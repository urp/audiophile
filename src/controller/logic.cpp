# include "controller/logic.hpp"

# include <algorithm>

using namespace std;

::controller::Logic::Logic( const shared_ptr< ::model::Game >& g ) : _model( g ) {}

void ::controller::Logic::addGameObject( shared_ptr< model::GameObject > const& o )
{
  // we do lazy adding in case of inserting an element while iterating on the object vector
  if( not o ) throw logic_error( "model::Game::addObject: Invalid object." );
  _waitingObjects.push_back( o );
}

bool ::controller::Logic::advance_model( const ::controller::InputEventHandler::keyboard_event& ev )
{
  game_model()->setTimestamp( chrono::steady_clock::now() );

  //clog << "::controller::Logic::advance: timestep " << chrono::duration_cast< chrono::milliseconds >( game_model()->timestep() ).count() << '.' << endl;

  preprocess( ev );

  for( auto o : game_model()->objects() )
  {
    if( o->is_dynamic() )
    {
      auto obj_logic = o->getData< ObjectLogic >();
      if( not obj_logic )
      {
        clog << "::controller::Logic::advance_model: Adding new ObjectLogic for \"" << o->name() << "\"." << endl;
        try{
          obj_logic = _obj_logic_factory.create_for( o );
        }catch( out_of_range e )
        {
          //cerr << "::controller::Logic::advance_model: cought exeption: " << e.what() << endl;
          continue;
        }

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
  auto predicate = []( shared_ptr< model::GameObject > const& go )->bool { return go->is_marked_as_deleted(); }; 
  game_model()->_objects.erase( remove_if( game_model()->_objects.begin(), game_model()->_objects.end(), predicate )
  , game_model()->_objects.end()
  );
}
