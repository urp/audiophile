# include "cg3preview/view/renderbuffer_al_audible.hpp"

# include <AL/alut.h>

using namespace ::cg3preview::view;

RenderbufferAlAudible::RenderbufferAlAudible( std::shared_ptr< model::Renderbuffer > const& b )
: _renderbuffer( b )
{
  // hint sounds
  alGenBuffers(2, _hint_buffer );
  alGenSources(2, _hint_source );

  _hint_buffer[0] = alutCreateBufferFromFile("../res/bling_low.wav");
  alSourcei ( _hint_source[0], AL_BUFFER, _hint_buffer[0] );
  alSource3f( _hint_source[0], AL_POSITION,  0.,0.,0. );
  alSourcef ( _hint_source[0], AL_GAIN, 1.0 );

  _hint_buffer[1] = alutCreateBufferFromFile("../res/bling_high.wav");
  alSourcei ( _hint_source[1], AL_BUFFER, _hint_buffer[1] );
  alSource3f( _hint_source[1], AL_POSITION,  0.,0.,0. );
  alSourcef ( _hint_source[1], AL_GAIN, 1.0 );

}

void RenderbufferAlAudible::auralize( ::view::AlRenderer& renderer )
{
  if( _renderbuffer->has_updates() )
    alSourcePlay( _hint_source[0] );

  static bool sound_played = false;
  if( not sound_played and _renderbuffer->is_complete() )
  { alSourcePlay( _hint_source[1] );
    sound_played = true;
  }
}
