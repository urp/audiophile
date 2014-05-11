# include "controller/default_object_logic.hpp"
# include <AL/alut.h>
# include <thread>

using namespace audiophile::controller;

DefaultObjectLogic::DefaultObjectLogic( const std::shared_ptr< model::GameObject >& o )
: Logic::ObjectLogic()
, _model( std::dynamic_pointer_cast< model::DefaultGameObject >( o ) )
{
  if( not _model )
    throw std::invalid_argument( "DefaultObjectLogic::DefaultObjectLogic: Could not cast GameObject to DefaultGameObject." );
}

void DefaultObjectLogic::advance( Logic& l )
{
  std::chrono::milliseconds rotation_interval( 5000 );

  double new_angle = 360. * ( std::chrono::duration_cast<std::chrono::milliseconds>( l.game_model()->timestamp.time_since_epoch() ) % rotation_interval ).count() / double( rotation_interval.count() );  
  _model->setAngle( new_angle );
}

bool DefaultObjectLogic::handle( const InputEventHandler::keyboard_event& ev )
{
  if( ev.key == InputEventHandler::keyboard_event::KEY_A )
  { 
    std::cout << "A pressed" << std::endl;
    {
      ALuint helloBuffer, helloSource;
      helloBuffer = alutCreateBufferHelloWorld ();
      alGenSources (1, &helloSource);
      alSourcei (helloSource, AL_BUFFER, helloBuffer);
      alSourcePlay (helloSource);
      //alutSleep (1);
    }
    std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
    {
      ALuint helloBuffer, helloSource;
      helloBuffer = alutCreateBufferHelloWorld ();
      alGenSources (1, &helloSource);
      alSourcei (helloSource, AL_BUFFER, helloBuffer);
      alSourcePlay (helloSource);
      alutSleep (1);
    }
    
  }
  
  return false;
}
