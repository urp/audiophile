# pragma once

# include "controller/engine.hpp"

# include <chrono>

namespace audiophile
{
  namespace controller 
  {

    class GlutEngine : public Engine, public std::enable_shared_from_this< GlutEngine >
    {
      public:
        GlutEngine( const std::shared_ptr< Logic >& );

        virtual void init( int&, char** ) override;
        virtual void run() override;

      protected:
        virtual bool handleKeyboardEvent( const InputEventHandler::keyboard_event& ) override;
    };

  } // controller::
} // audiophile::