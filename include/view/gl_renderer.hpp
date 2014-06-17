# pragma once

# include "model/game.hpp"

# include "factory_map.hpp"

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
      GlRenderer( std::shared_ptr< model::Game const > const& );

      std::shared_ptr< model::Game const > const& game_model() const;

      /// Return factory creating Drawable delegates.
      delegate_factory_type&       drawable_factory();
      delegate_factory_type const& drawable_factory() const;

      virtual void visualize_model( GlutWindow& );
      virtual void resize( GlutWindow& );

    private:
      std::shared_ptr< model::Game const > _game_model;
      delegate_factory_type _drawable_factory;

  }; // GlRenderer

} // view::

