# include "flappy_box/view/box_al_audible.hpp"

# include <AL/alut.h>

using namespace ::flappy_box::view;

BoxAlAudible::BoxAlAudible( std::shared_ptr< model::Box > const& b )
: _model( b )
{
  ALuint al_target_source;
  alGenSources( 1, & al_target_source );
  alSource3f( al_target_source, AL_POSITION, -0.5, 0.0, 0.0    );

  alGenSources( 1, &_al_player_source );
  alSource3f( _al_player_source, AL_POSITION, 0.5, 0.0, 0.0     );

  ALuint al_target_buffer;
  alGenBuffers( 1, &al_target_buffer );
  al_target_buffer = alutCreateBufferWaveform( ALUT_WAVEFORM_SINE, target_frequency, 0, 200 );
  alSourcei ( al_target_source, AL_BUFFER, al_target_buffer );

  ALuint al_player_buffer;
  alGenBuffers( 1, &al_player_buffer );
  al_player_buffer = alutCreateBufferWaveform( ALUT_WAVEFORM_SINE, base_frequency, 0, 100 );
  alSourcei ( _al_player_source, AL_BUFFER, al_player_buffer );

  alSourcePlay( al_target_source );
  alSourcePlay( _al_player_source );
}

void BoxAlAudible::auralize( ::view::AlRenderer& renderer )
{
  ALfloat player_frequency =  fabs( _model->position().z() ) * 10.0 + 100.0;
  alSourcef( _al_player_source, AL_PITCH, player_frequency / base_frequency );
}
