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

  double new_angle = 360. * ( std::chrono::duration_cast<std::chrono::milliseconds>( l.game_model()->timestamp().time_since_epoch() ) % rotation_interval ).count() / double( rotation_interval.count() );  
  _model->setAngle( new_angle );

  double timestep_sec = l.game_model()->timestep().count();

  vec3_type acceleration = _model->acceleration();
  acceleration -= 10 * acceleration * timestep_sec;

  if( ev.key == ::controller::InputEventHandler::keyboard_event::KEY_A )
  {
    acceleration[2] += 20.;
    std::cout << "flappy_box::controller::BoxObjectLogic::handle: Key A pressed." << std::endl;
  }

  // Euler integration
  const vec3_type gravity( 0.,0., -2. );
  _model->setAcceleration( acceleration );
  _model->setVelocity( _model->velocity() + ( _model->acceleration() + gravity ) * timestep_sec );
  _model->setPosition( _model->position() + _model->velocity() * timestep_sec );

  std::cout << "flappy_box::controller::BoxObjectLogic::advance:"
            << " dt  " << timestep_sec << " p " << _model->position()[2] << " v " << _model->velocity()[2] <<  " a " << acceleration[2]
            << std::endl;

  return false;
}