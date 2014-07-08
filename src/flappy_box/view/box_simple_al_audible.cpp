# include "flappy_box/view/box_al_audible.hpp"

# include <AL/alut.h>

using namespace ::flappy_box::view;

BoxAlAudible::BoxAlAudible( std::shared_ptr< model::Box > const& b )
: _box( b )
{
  // al box source
  alGenSources( 1, &_al_box_source );
  alSource3f( _al_box_source, AL_POSITION, 0.5, 0.0, 0.0     );
  alSourcei( _al_box_source, AL_LOOPING, AL_TRUE );

  ALuint al_target_source;
  alGenSources( 1, & al_target_source );
  alSource3f( al_target_source, AL_POSITION, -0.5, 0.0, 0.0    );
  alSourcei( al_target_source, AL_LOOPING, AL_TRUE );

  ALuint al_box_buffer;
  alGenBuffers( 1, &al_box_buffer );
  al_box_buffer = alutCreateBufferWaveform( ALUT_WAVEFORM_SINE, base_frequency, 0, 10. );
  alSourcei ( _al_box_source, AL_BUFFER, al_box_buffer );

  ALuint al_target_buffer;
  alGenBuffers( 1, &al_target_buffer );
  al_target_buffer = alutCreateBufferWaveform( ALUT_WAVEFORM_SINE, target_frequency, 0, 10. );
  alSourcei ( al_target_source, AL_BUFFER, al_target_buffer );

  alSourcePlay( _al_box_source );
  alSourcePlay( al_target_source );
}

void BoxAlAudible::auralize( ::view::AlRenderer& renderer )
{
  ALfloat box_frequency =  fabs( _box->position().z() ) * 10.0 + target_frequency;
  alSourcef( _al_box_source, AL_PITCH, box_frequency / base_frequency );
}
