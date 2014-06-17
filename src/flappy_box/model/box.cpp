# include "flappy_box/model/box.hpp"

using namespace flappy_box::model;

Box::Box( std::shared_ptr< Tube const > const& t, double const& d, std::string const& n )
: GameObject( true, n )
, _diameter( d )
, _angle( 0. )
, _position( 0.,0.,0. )
, _velocity( 0.,0.,0. )
, _acceleration( 0.,0.,0. )
, _tube( t )
{

}

// ::sphere< ::distance_type, 2 > Box::getBoundingSphere() const
// {
// 
// }
// 
// ::box< ::distance_type, 2 > Box::getBoundingBox() const
// {
// 
// }
