# include "view/al_renderer.hpp"

using namespace ::view;

void AlRenderer::auralize_model()
{
  for( auto o : game_model()->objects() )
  {
    std::shared_ptr< Audible > audible = o->getData< Audible >();
    if( not audible )
    {
      std::clog << "::view::AlRenderer::draw: Adding new Audible for \"" << o->name() << "\"." << std::endl;
      audible = _audible_factory.create_for( o );
      o->registerData( audible );
    }

    if( audible ) audible->auralize( *this );
  } 
}
