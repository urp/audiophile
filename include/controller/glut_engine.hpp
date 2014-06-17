# pragma once

# include "controller/engine.hpp"

# include <chrono>

namespace controller 
{

  class GlutEngine : public Engine
  {
    public:
      GlutEngine( const std::shared_ptr< Logic >& = std::make_shared< ::controller::Logic >( std::make_shared< ::model::Game >() ) );

      virtual void init( int& argc, char** argv ) override;
      virtual void run() override;
    protected:
      // time interval in which the (virtual) Engine::step function should be called.
      unsigned int _prefered_timestep_millisec = 30;

  }; // GlutEngine

} // controller::
