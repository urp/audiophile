# pragma once

# include "view/al_renderer.hpp"
# include "cg3preview/model/renderbuffer.hpp"

namespace cg3preview
{
  namespace view
  {
    class RenderbufferAlAudible : public ::view::AlRenderer::Audible
    {
      public:
        RenderbufferAlAudible( std::shared_ptr< model::Renderbuffer > const& );

        virtual void auralize( ::view::AlRenderer& ) override;

      private:
        std::shared_ptr< model::Renderbuffer const > _renderbuffer;

        ALuint _hint_buffer[2];
        ALuint _hint_source[2];

    }; // RenderbufferAlAudible

  } // view::
} // cg3preview::
