# include "controller/engine.hpp"

using namespace ::controller;

Engine::Engine( const std::shared_ptr< Logic >& l
              , const std::shared_ptr< ::model::Game >& g 
              )
: _model( g ), _logic( l )
{

}

std::shared_ptr< ::model::Game > Engine::game_model()
{
  return _model;
}

std::shared_ptr< const ::model::Game > Engine::game_model() const
{
  return _model;
}

std::shared_ptr< Logic > Engine::game_logic()
{
  return _logic;
}

std::shared_ptr< const Logic > Engine::game_logic() const
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
