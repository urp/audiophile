# pragma once

# include "controller/logic.hpp"
# include "flappy_box/model/tube.hpp"

namespace flappy_box
{
  namespace controller
  {

    class TubeObjectLogic : public ::controller::Logic::ObjectLogic
    {
      public:
        TubeObjectLogic() = delete;
        TubeObjectLogic( const std::shared_ptr< model::Tube >& );

        virtual bool advance( ::controller::Logic&, ::controller::InputEventHandler::keyboard_event const & ) override;

      private:
        std::shared_ptr< model::Tube > _model;

    }; // TubeObjectLogic

  } // controller::

} // flappy_box::