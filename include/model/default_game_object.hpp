# pragma once

# include "model/game_object.hpp"

namespace audiophile
{

  namespace model
  {
    class DefaultGameObject : public GameObject
    {
      public:

        DefaultGameObject(const std::string& = "untitled DefaultGameObject");

        virtual sphere< distance_type, 2 >      getBoundingSphere() const override;
        virtual box< distance_type, 2 >         getBoundingBox()    const override;

    }; // DefaultGameObject

  } // model::
} // audiophile::