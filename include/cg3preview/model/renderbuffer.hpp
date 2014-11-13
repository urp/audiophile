# pragma once

# include "model/game_object.hpp"

# include <cg3/manager/render_manager.hpp>

# include <queue>
# include <mutex>

namespace cg3preview
{
  namespace model
  {

    class Renderbuffer : public ::model::GameObject
    {
      public:
        Renderbuffer( std::shared_ptr< RenderManager > const& rm );

        std::shared_ptr< RenderManager > get_render_manager() { return _render_manager; }

        void push_updated_tile( Tile const& t )
        {
          std::lock_guard< std::mutex > lock_updated_tiles( _updated_tiles_mutex );
          _updated_tiles.push( std::move( t ) );
        }

        bool has_updates() const
        {
          std::lock_guard< std::mutex > lock_updated_tiles( _updated_tiles_mutex );
          return not _updated_tiles.empty();
        }

        Tile pop_updated_tile()
        {
          std::lock_guard< std::mutex > lock_updated_tiles( _updated_tiles_mutex );
          Tile t = std::move( _updated_tiles.front() );
          _updated_tiles.pop();
          return t;
        }

        bool is_complete() const { return _is_complete; }
        void mark_as_complete( bool c = true ) { _is_complete = c; }

      private:
        std::shared_ptr< RenderManager > _render_manager;
        std::queue< Tile > _updated_tiles;
        bool _is_complete;
        mutable std::mutex  _updated_tiles_mutex;

    }; // Renderbuffer

  } // model::
} // cg3preview::