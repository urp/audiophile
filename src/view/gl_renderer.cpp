# include "view/gl_renderer.hpp"

# include "view/glut_window.hpp"

using namespace audiophile::view;

GLRenderer::GLRenderer( const std::shared_ptr< const model::Game >& g, const std::string n )
: _game_model( g )
, _name( n )
{}

std::shared_ptr< const audiophile::model::Game > GLRenderer::game_model() const
{
  return _game_model;
}


std::string GLRenderer::name() const
{
  return _name;
}