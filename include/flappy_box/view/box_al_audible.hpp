# pragma once

# include "view/al_renderer.hpp"

# include "flappy_box/model/box.hpp"

namespace flappy_box
{
  namespace view
  {
    class BoxAlAudible : public ::view::AlRenderer::Audible
    {
      public:
        const double target_frequency = 100.0;
        const double base_frequency = 400.0;

        BoxAlAudible( const std::shared_ptr< model::Box >& );

        virtual void auralize( ::view::AlRenderer& ) override;

      private:
        std::shared_ptr< model::Box > _model;
        ALuint _al_player_source;
    };
  }
}
