# pragma once

# include "model/game.hpp"

# include <string>
# include <AL/al.h>

namespace view 
{
  class AlRenderer
  {
    public:
      struct Buffer
      {
        ALuint al_handle;
      };

      struct Source
      {
        Source();
        Source( const shared_ptr< Buffer >& );

        ALuint m_al_handle;

        vec3_type m_position;
        vec3_type m_velocity;
      };

      AlRenderer() = delete;
      AlRenderer( const std::shared_ptr< const model::Game >&, std::string name = "AlRenderer" );

      std::shared_ptr< const model::Game > game_model() const;
      std::string name() const;

      void auralize_model();

    private:
      std::string _name;
      std::shared_ptr< const model::Game > _game_model;

      std::map< std::string, Buffer > _cached_buffers;
  }; // Renderer

} // view::

