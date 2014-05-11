# pragma once

# include "controller/engine.hpp"

# include <chrono>

namespace controller 
{

  class GlutEngine : public Engine
  {
    public:
      GlutEngine( const std::shared_ptr< Logic >& );

      virtual void init( int&, char** ) override;
      virtual void run() override;

      const unsigned int _prefered_timestep_millisec = 30;
  };

} // controller::
