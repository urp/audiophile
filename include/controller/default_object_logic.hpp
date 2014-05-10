# pragma once

# include "controller/logic.hpp"
# include "model/default_game_object.hpp"

namespace audiophile 
{
  namespace controller
  {

    class DefaultObjectLogic : public Logic::ObjectLogic
    {
      public:
        DefaultObjectLogic() = delete;
        DefaultObjectLogic( const std::shared_ptr< model::GameObject >& );

        virtual void advance( Logic& ) override;

      private:
        std::shared_ptr< model::DefaultGameObject > _model;
    };

  }

} // audiophile