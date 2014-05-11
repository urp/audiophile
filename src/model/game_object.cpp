# include "model/game_object.hpp"

using namespace ::model;

::model::GameObject::GameObject( const bool dynamic_flag, const std::string& name )
: _dynamic_flag( dynamic_flag ), _name( name )
{

}
