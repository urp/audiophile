# include "flappy_box/model/box.hpp"

using namespace flappy_box::model;

Box::Box( const std::string& n )
: GameObject( true, n )
{

}

::sphere< ::distance_type, 2 > Box::getBoundingSphere() const
{

}

::box< ::distance_type, 2 > Box::getBoundingBox() const
{

}
