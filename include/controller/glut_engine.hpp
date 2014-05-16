# pragma once

# include "controller/engine.hpp"

# include <GL/freeglut.h>

# include <chrono>

namespace controller 
{

  class GlutEngine : public Engine
  {
    public:
      GlutEngine( const std::shared_ptr< Logic >& );

      virtual void init( int&, char** ) override;
      virtual void run() override;
    private:
      const unsigned int _prefered_timestep_millisec = 30;

  }; // GlutEngine

} // controller::
