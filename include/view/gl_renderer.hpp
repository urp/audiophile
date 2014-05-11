# pragma once

# include "model/game.hpp"

# include <string>

namespace view 
{
  class GlutWindow;

  class GlRenderer
  {
    public:
      class Drawable
      {
        public:
          virtual void glDraw( GlRenderer&, GlutWindow& ) = 0;
      };

      typedef factory_map< model::GameObject, Drawable > factory_type;

      GlRenderer() = delete;
      GlRenderer( const std::shared_ptr< const model::Game >&, std::string name = "GlRenderer" );

      std::shared_ptr< const model::Game > game_model() const;
      std::string name() const;

      factory_type&       drawable_factory()       { return _drawable_factory; }
      factory_type const& drawable_factory() const { return _drawable_factory; }

      virtual void initialize( GlutWindow& );
      virtual void draw( GlutWindow& );
      virtual void resize( GlutWindow& );

    private:
      std::string _name;
      std::shared_ptr< const model::Game > _game_model;
      factory_type _drawable_factory;
 
  }; // Renderer

} // view::

