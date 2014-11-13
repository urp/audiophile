# pragma once

# include "controller/logic.hpp"
# include "cg3preview/model/renderbuffer.hpp"

namespace cg3preview
{
  namespace controller
  {

    class RenderbufferObjectLogic : public ::controller::Logic::ObjectLogic
    {
      public:
        RenderbufferObjectLogic() = delete;
        RenderbufferObjectLogic( const std::shared_ptr< model::Renderbuffer >& );
        ~RenderbufferObjectLogic();
        virtual bool advance( ::controller::Logic&, ::controller::InputEventHandler::keyboard_event const & ) override;

      private:
        std::thread _render_thread;
        std::shared_ptr< model::Renderbuffer > _renderbuffer;

    }; // RenderbufferObjectLogic

  } // controller::

} // cg3preview::