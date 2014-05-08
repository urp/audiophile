# pragma once

# include "controller/engine.hpp"

# include <chrono>

namespace audiophile
{
  namespace controller 
  {

    class DefaultEngine : public Engine, public std::enable_shared_from_this< DefaultEngine >
    {
      public:
        DefaultEngine();

        virtual void init( int&, char** ) override;
        virtual void run() override;

      protected:
        virtual bool handleKeyboardEvent( const InputEventHandler::keyboard_event& ) override;

        static void glutIdle();

      private:
        std::chrono::system_clock::time_point _current_time;
    };

  } // controller::
} // audiophile::