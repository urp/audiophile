
# include "controller/engine.hpp"

using namespace audiophile::controller;

Engine::Engine( const std::shared_ptr< audiophile::model::Game >& g )
: _game_model( g )
{

}


void Engine::addView( const std::shared_ptr< audiophile::view::GLUTWindow >& v )
{
  _views.push_back( v );
}

const std::vector< std::shared_ptr< audiophile::view::GLUTWindow > >& Engine::views() const
{
  return _views;
}

std::shared_ptr< audiophile::model::Game > Engine::game_model()
{
  return _game_model;
}

std::shared_ptr< const audiophile::model::Game > Engine::game_model() const
{
  return _game_model;
}