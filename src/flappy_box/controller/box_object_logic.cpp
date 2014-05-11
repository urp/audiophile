# include "flappy_box/controller/box_object_logic.hpp"
# include <AL/alut.h>
# include <thread>

using namespace ::flappy_box::controller;

BoxObjectLogic::BoxObjectLogic(const std::shared_ptr< ::model::GameObject >& o )
: ::controller::Logic::ObjectLogic()
, _model( std::dynamic_pointer_cast< model::Box >( o ) )
{
  if( not _model )
    throw std::invalid_argument( "flappy_box::controller::BoxObjectLogic::BoxObjectLogic: Could not cast GameObject to flappy_box::model::Box." );
}

void BoxObjectLogic::advance( ::controller::Logic& l )
{
  const std::chrono::milliseconds rotation_interval( 5000 );

  double new_angle = 360. * ( std::chrono::duration_cast<std::chrono::milliseconds>( l.game_model()->timestamp.time_since_epoch() ) % rotation_interval ).count() / double( rotation_interval.count() );  
  _model->setAngle( new_angle );
}

bool BoxObjectLogic::handle( const ::controller::InputEventHandler::keyboard_event& ev )
{
  if( ev.key == ::controller::InputEventHandler::keyboard_event::KEY_A )
  { 
    std::cout << "flappy_box::controller::BoxObjectLogic::handle: Key A pressed" << std::endl;
    {
      ALuint helloBuffer, helloSource;
      helloBuffer = alutCreateBufferHelloWorld ();
      alGenSources (1, &helloSource);
      alSourcei (helloSource, AL_BUFFER, helloBuffer);
      alSourcePlay (helloSource);
      //alutSleep (1);
    }
  }
  
  return false;
}
