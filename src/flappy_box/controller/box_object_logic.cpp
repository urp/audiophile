# include "flappy_box/controller/box_object_logic.hpp"
# include <AL/alut.h>
# include <thread>

using namespace ::flappy_box::controller;

BoxObjectLogic::BoxObjectLogic(const std::shared_ptr< model::Box >& b )
: ::controller::Logic::ObjectLogic()
, _model( b )
{}

bool BoxObjectLogic::advance( ::controller::Logic& l, ::controller::InputEventHandler::keyboard_event const& ev )
{
  const std::chrono::milliseconds rotation_interval( 5000 );

  double new_angle = 360. * ( std::chrono::duration_cast<std::chrono::milliseconds>( l.game_model()->timestamp.time_since_epoch() ) % rotation_interval ).count() / double( rotation_interval.count() );  
  _model->setAngle( new_angle );

  double timestep_sec = l.game_model()->timestep.count();
  const vec3_type gravity( 0.,0., -2. );
  static vec3_type flapping( 0.,0., 0.0 );

  flapping[2] -= 10 * flapping[2] * timestep_sec;
  if( ev.key == ::controller::InputEventHandler::keyboard_event::KEY_A )
  { 
    flapping[2] += 20.;
    std::cout << "flappy_box::controller::BoxObjectLogic::handle: Key A pressed." << std::endl;
    {
      ALuint helloBuffer, helloSource;
      helloBuffer = alutCreateBufferHelloWorld ();
      alGenSources (1, &helloSource);
      alSourcei (helloSource, AL_BUFFER, helloBuffer);
      alSourcePlay (helloSource);
      //alutSleep (1);
    }
    
    
  }
  
  _model->setVelocity( _model->velocity() + ( gravity + flapping ) * timestep_sec );
  _model->setPosition( _model->position() + _model->velocity() * timestep_sec );

  std::cout << "flappy_box::controller::BoxObjectLogic::advance:"
  << " dt  " << timestep_sec << " flap " << flapping[2]<< std::endl
            << " pos " << std::endl << _model->position() << std::endl
            << " vel " << std::endl << _model->velocity() << std::endl;

  return false;
}