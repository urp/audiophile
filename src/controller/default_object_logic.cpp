#include "controller/default_object_logic.hpp"

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

