# include "controller/default_logic.hpp"

# include "controller/default_object_logic.hpp"

audiophile::controller::DefaultLogic::DefaultLogic(const std::shared_ptr< model::Game >& g)
: Logic( g )
{
  _obj_logic_factory->register_module< model::DefaultGameObject >( []( const std::shared_ptr< model::GameObject >& p ) { return std::make_shared< DefaultObjectLogic >( p ); } );
}