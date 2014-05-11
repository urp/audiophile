# pragma once

# include "controller/logic.hpp"
# include "flappy_box/model/box.hpp"

namespace flappy_box
{
  namespace controller
  {

    class BoxObjectLogic : public ::controller::Logic::ObjectLogic
    {
      public:
        BoxObjectLogic() = delete;
        BoxObjectLogic( const std::shared_ptr< model::Box >& );

        virtual bool advance( ::controller::Logic&, ::controller::InputEventHandler::keyboard_event const & ) override;

      private:
        std::shared_ptr< model::Box > _model;
    };

  }

} // audiophile