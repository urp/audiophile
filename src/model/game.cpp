# include "model/game.hpp"

using namespace model;

Game::Game() 
: _timestamp( std::chrono::steady_clock::now() )
, _runtime( 0. )
{ }

void Game::addGameObject( std::shared_ptr< GameObject > const& o )
{
  if( not o ) throw std::logic_error( "model::Game::addObject: Invalid object." );
  _objects.push_back( o );
}

const std::vector< std::shared_ptr< GameObject > >&  Game::objects() const 
{
  return _objects;
}

std::chrono::steady_clock::time_point const& Game::timestamp() const 
{
  return _timestamp;
}

std::chrono::duration< double > const& Game::timestep() const 
{
  return _timestep;
}

void Game::setTimestamp( std::chrono::steady_clock::time_point const& t )
{
  auto last_timestamp = timestamp();
  _timestamp = t;
  _timestep = t - last_timestamp;
  _runtime += _timestep;
}
