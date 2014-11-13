# include "view/al_renderer.hpp"

using namespace ::view;

AlRenderer::AlRenderer( std::shared_ptr< model::Game const > const& g ) 
: _game_model( g ) 
{}

std::shared_ptr< model::Game const > const& AlRenderer::game_model() const
{
  return _game_model;
}

AlRenderer::delegate_factory_type& AlRenderer::audible_factory()
{
  return _audible_factory;
}

AlRenderer::delegate_factory_type const& AlRenderer::audible_factory() const
{
  return _audible_factory;
}

void AlRenderer::auralize_model()
{
  for( auto o : game_model()->objects() )
  {
    std::shared_ptr< Audible > audible = o->getData< Audible >();
    if( not audible )
    {
      std::clog << "::view::AlRenderer::auralize_model: Adding new Audible for \"" << o->name() << "\"." << std::endl;
      try
      {
        audible = _audible_factory.create_for( o );
      }catch( std::out_of_range e )
      {
        //std::cerr << "::view::GlRenderer::auralize_model: cought exeption: " << e.what() << std::endl;
        continue;
      }

      o->registerData( audible );
    }

    if( audible ) audible->auralize( *this );
  } 
}
