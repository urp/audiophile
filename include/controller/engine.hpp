
# pragma once

# include "model/game.hpp"
# include "controller/input_event_handler.hpp"
# include "view/glut_window.hpp"

# include <chrono>

namespace audiophile
{
  namespace controller 
  {
    /// Application class
    class Engine : public InputEventHandler
    {
      public:
        Engine( const std::shared_ptr< model::Game >& = std::shared_ptr< model::Game >( new model::Game ) );

        virtual void init( int&, char** ) = 0;
        virtual void run() = 0;

        std::shared_ptr< model::Game >       game_model();
        std::shared_ptr< const model::Game > game_model() const;

      protected:
        void addView( const std::shared_ptr< view::GLUTWindow >& );
        const std::vector< std::shared_ptr< view::GLUTWindow > >& views() const;

      private:
        std::shared_ptr< model::Game > _game_model;
        std::vector< std::shared_ptr< view::GLUTWindow > > _views;
    };

  } // controller::
} // audiophile::