# pragma once

# include "model/game.hpp"

# include "math.hpp"
# include "factory_map.hpp"

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
        Source( const std::shared_ptr< Buffer >& );

        ALuint m_al_handle;

        vec3_type m_position;
        vec3_type m_velocity;
      };

      struct Audible : public ::model::GameObject::Data
      {
        virtual void auralize( AlRenderer& ) = 0;
      };

      typedef factory_map< model::GameObject, Audible > delegate_factory_type;

      AlRenderer() = delete;
      AlRenderer( const std::shared_ptr< const model::Game >& g ) : _game_model( g ) {};

      std::shared_ptr< const model::Game > game_model() const { return _game_model; }

      /// Return factory creating Audible delegates.
      delegate_factory_type&       audible_factory()       { return _audible_factory; }
      delegate_factory_type const& audible_factory() const { return _audible_factory; }

      void auralize_model();

    private:
      std::string _name;
      std::shared_ptr< const model::Game > _game_model;

      std::map< std::string, Buffer > _cached_buffers;

      delegate_factory_type _audible_factory;
  }; // AlRenderer

} // view::

