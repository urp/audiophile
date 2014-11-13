# include "cg3preview/controller/renderbuffer_object_logic.hpp"

using namespace ::cg3preview::controller;

RenderbufferObjectLogic::RenderbufferObjectLogic( std::shared_ptr< model::Renderbuffer > const& rb )
: ::controller::Logic::ObjectLogic()
, _renderbuffer( rb )
{
  _renderbuffer->get_render_manager()->signal_tile_completed.connect( [this]( Tile const t ){ this->_renderbuffer->push_updated_tile( t ); } );
  _renderbuffer->get_render_manager()->signal_image_completed.connect( [this]( Tile::image_type const ){ this->_renderbuffer->mark_as_complete( true ); } );

  _render_thread = thread( [this,rb](){ rb->get_render_manager()->render_image(); } );
}

RenderbufferObjectLogic::~RenderbufferObjectLogic()
{
  if( _render_thread.joinable() )
  {
    _renderbuffer->get_render_manager()->send_cancel_request();
    _render_thread.join();
  }
}

bool RenderbufferObjectLogic::advance( ::controller::Logic& l, ::controller::InputEventHandler::keyboard_event const& ev )
{
  //std::cout << "cg3preview::controller::RenderbufferObjectLogic::advance: doing nothing" << std::endl;
  return false;
}