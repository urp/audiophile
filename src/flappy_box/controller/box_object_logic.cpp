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

  auto timestamp = l.game_model()->timestamp();
  double timestep_sec = l.game_model()->timestep().count();

  double new_angle = 360. * ( std::chrono::duration_cast<std::chrono::milliseconds>( timestamp.time_since_epoch() ) % rotation_interval ).count() / double( rotation_interval.count() );
  _model->setAngle( new_angle );

  // Calculate forces

  vec3_type acceleration = _model->acceleration();
  acceleration -= 10 * acceleration * timestep_sec;

  if( ev.key == 'a' )
  {
    acceleration[2] += 20.;
    std::cout << "flappy_box::controller::BoxObjectLogic::handle: Key A pressed." << std::endl;
  }

  // Euler integration

  const vec3_type gravity( 0.,0., -1. );
  _model->setAcceleration( acceleration );
  _model->setVelocity( _model->velocity() + ( _model->acceleration() + gravity ) * timestep_sec );
  _model->setPosition( _model->position() + _model->velocity() * timestep_sec );

  // Collision check
  model::Tube::control_point_type  tube_bounds;
  try
  { tube_bounds = _model->tube_object()->getVerticalBoundsAt( timestamp ); }
  catch( std::out_of_range e )
  {
    std::cerr << "flappy_box::controller::BoxObjectLogic::advance: Unable to determine collision bounds: " << e.what() << std::endl;
    return false;
  }
  std::cout << "flappy_box::controller::BoxObjectLogic::advance: collision check tube_bounds (" << tube_bounds.first << ", " << tube_bounds.second << ")." << std::endl;
  const double bounce_factor = -.5;

  if( _model->position()[2] - .5 * _model->diameter() < tube_bounds.first )
  { auto pos = _model->position();
    pos[2] = tube_bounds.first + .55 * _model->diameter();
    _model->setPosition( pos );
    auto vel = _model->velocity();
    vel[2] *= bounce_factor;
    _model->setVelocity( vel );
    auto acc = _model->acceleration();
    acc[2] = 0.;
    _model->setAcceleration( acc );
  }

  if( _model->position()[2] + .5 * _model->diameter() > tube_bounds.second )
  { auto pos = _model->position();
    pos[2] = tube_bounds.second - .55 * _model->diameter();
    _model->setPosition( pos );
    auto vel = _model->velocity();
    vel[2] *= bounce_factor ;
    _model->setVelocity( vel );
    auto acc = _model->acceleration();
    acc[2] = 0.;
    _model->setAcceleration( acc );
  }

  std::cout << "flappy_box::controller::BoxObjectLogic::advance:"
            << " dt  " << timestep_sec << " p " << _model->position()[2] << " v " << _model->velocity()[2] <<  " a " << _model->acceleration()[2] 
            << std::endl;

  return false;
}