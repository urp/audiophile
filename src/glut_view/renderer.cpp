  # include "glut_view/renderer.hpp"

# include "glut_view/window.hpp"

using namespace audiophile::glut_view;

Renderer::Renderer( const std::shared_ptr< const model::Game >& g, const std::string n )
: _game_model( g )
, _name( n )
{}

std::shared_ptr< const audiophile::model::Game > Renderer::game_model() const
{
  return _game_model;
}


std::string Renderer::name() const
{
  return _name;
}