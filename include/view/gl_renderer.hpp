# pragma once

# include "model/game.hpp"

# include <string>

namespace view 
{
  class GlutWindow;

  class GlRenderer
  {
    public:
      struct Drawable : public ::model::GameObject::Data
      {
        virtual void visualize( GlRenderer&, GlutWindow& ) = 0;
      };

      typedef factory_map< model::GameObject, Drawable > delegate_factory_type;

      GlRenderer() = delete;
      GlRenderer( const std::shared_ptr< const model::Game >& );

      std::shared_ptr< const model::Game > game_model() const;

      /// Return factory creating Drawable delegates.
      delegate_factory_type&       drawable_factory()       { return _drawable_factory; }
      delegate_factory_type const& drawable_factory() const { return _drawable_factory; }

      virtual void initialize( GlutWindow& );
      virtual void draw( GlutWindow& );
      virtual void resize( GlutWindow& );

    private:
      std::shared_ptr< const model::Game > _game_model;
      delegate_factory_type _drawable_factory;

  }; // GlRenderer

} // view::

