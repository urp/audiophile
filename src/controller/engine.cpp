# include "controller/engine.hpp"

using namespace ::controller;

Engine::Engine( const std::shared_ptr< Logic >& l )
: _model( l->game_model() )
, _logic( l )
{}

std::shared_ptr< ::model::Game >& Engine::game_model()
{
  return _model;
}

std::shared_ptr< ::model::Game const > Engine::game_model() const
{
  return _model;
}

std::shared_ptr< Logic >& Engine::game_logic()
{
  return _logic;
}

std::shared_ptr< Logic const > Engine::game_logic() const
{
  return _logic;
}

void Engine::step( InputEventHandler::keyboard_event const& ev )
{
  game_logic()->advance_model( ev );
}

bool Engine::handle( ::controller::InputEventHandler::keyboard_event const& ev )
{
  step( ev );
}
