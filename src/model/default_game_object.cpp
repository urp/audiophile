# include "model/default_game_object.hpp"

# include <chrono>

using namespace audiophile::model;


DefaultGameObject::DefaultGameObject( const std::string& n )
: GameObject( true, n )
{

}

audiophile::sphere< audiophile::distance_type, 2 > DefaultGameObject::getBoundingSphere() const
{

}

audiophile::box< audiophile::distance_type, 2 > DefaultGameObject::getBoundingBox() const
{

}
