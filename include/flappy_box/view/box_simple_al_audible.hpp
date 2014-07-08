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
        double const target_frequency = 100.0;
        double const base_frequency = 400.0;

        BoxAlAudible( std::shared_ptr< model::Box > const& );

        virtual void auralize( ::view::AlRenderer& ) override;

      private:
        std::shared_ptr< model::Box const > _box;
        ALuint _al_box_source;

    }; // BoxAlAudible

  } // view::
} // flappy_box
