# include "flappy_box/controller/tube_object_logic.hpp"

using namespace ::flappy_box::controller;

using ::flappy_box::model::Tube;

TubeObjectLogic::TubeObjectLogic(const std::shared_ptr< model::Tube >& t )
: ::controller::Logic::ObjectLogic()
, _model( t )
{}

bool TubeObjectLogic::advance( ::controller::Logic& l, ::controller::InputEventHandler::keyboard_event const& ev )
{
  auto const& timestamp = l.game_model()->timestamp();

  _model->manageControlPoints( timestamp );

  return false;
}