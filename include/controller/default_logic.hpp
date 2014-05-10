# pragma once

# include "controller/logic.hpp"

namespace audiophile
{
  namespace controller
  {
    class DefaultLogic : public Logic
    {
      public:

        DefaultLogic( const std::shared_ptr< model::Game >& );

        //virtual void handleKeyboard( const controller::InputEventHandler::keyboard_event& );

    };

  };
}