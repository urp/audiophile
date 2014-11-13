# include "cg3preview/model/renderbuffer.hpp"

using namespace cg3preview::model;

Renderbuffer::Renderbuffer( std::shared_ptr< RenderManager > const& rm )
: GameObject( true, "Renderbuffer" ), _render_manager( rm ), _is_complete( false )
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
