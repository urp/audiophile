# include "model/default_game_object.hpp"

using namespace audiophile::model;


DefaultGameObject::DefaultGameObject( const std::string& n )
: GameObject( n )
{

  

}

audiophile::sphere< audiophile::distance_type, 2 > DefaultGameObject::getBoundingSphere() const
{

}

audiophile::box< audiophile::distance_type, 2 > DefaultGameObject::getBoundingBox() const
{

}

