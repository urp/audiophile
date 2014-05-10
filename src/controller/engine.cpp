
# include "controller/engine.hpp"

using namespace audiophile::controller;

Engine::Engine( const std::shared_ptr< Logic >& l
              , const std::shared_ptr< audiophile::model::Game >& g 
              )
: _model( g ), _logic( l )
{

}

std::shared_ptr< audiophile::model::Game > Engine::game_model()
{
  return _model;
}

std::shared_ptr< const audiophile::model::Game > Engine::game_model() const
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