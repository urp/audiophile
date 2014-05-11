# pragma once

# include "controller/engine.hpp"

# include <chrono>

namespace audiophile
{
  namespace controller 
  {

    class GlutEngine : public Engine
    {
      public:
        GlutEngine( const std::shared_ptr< Logic >& );

        virtual void init( int&, char** ) override;
        virtual void run() override;

      protected:
    };

  } // controller::
} // audiophile::