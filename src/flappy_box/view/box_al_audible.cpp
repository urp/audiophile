# include "flappy_box/view/box_al_audible.hpp"

# include <AL/alut.h>

using namespace ::flappy_box::view;

BoxAlAudible::BoxAlAudible( std::shared_ptr< model::Box > const& b )
: _box( b )
{
  // create white noise
  alGenSources (1, &whiteNoiseSource);
  alGenBuffers(1, &whiteNoiseBuffer);
  whiteNoiseBuffer = alutCreateBufferWaveform(ALUT_WAVEFORM_WHITENOISE, 1000.0, 0, 1);
  alSourcei(whiteNoiseSource, AL_BUFFER, whiteNoiseBuffer);
  alSourcef(whiteNoiseSource, AL_GAIN, 0.0 );
  alSourcei(whiteNoiseSource, AL_LOOPING, AL_TRUE);
  alSource3f(whiteNoiseSource, AL_POSITION, 0.0, 0.0, 0.0);
  alSourcePlay(whiteNoiseSource);
  
  // music part
  alGenSources(4, musicSources);
  alGenBuffers(4, musicBuffers);
  musicBuffers[0] = alutCreateBufferFromFile("../res/low.wav");
  musicBuffers[1] = alutCreateBufferFromFile("../res/high.wav");
  musicBuffers[2] = alutCreateBufferFromFile("../res/beat.wav");
  musicBuffers[3] = alutCreateBufferFromFile("../res/synth.wav");
  //std::cout<<alutGetErrorString(alutGetError())<<std::endl;

  // panning of clave-like patterns (beat and synth stay in the center)
  alSource3f(musicSources[0], AL_POSITION, -1.0, 0.0, 0.0);
  alSource3f(musicSources[1], AL_POSITION,  1.0, 0.0, 0.0);
  
  for( int i = 0; i < 4; i++)
  {
    alSourcei(musicSources[i], AL_BUFFER, musicBuffers[i]);
    alSourcef(musicSources[i], AL_GAIN, 0.0 );
    alSourcei(musicSources[i], AL_LOOPING, AL_TRUE);
    alSourcePlay(musicSources[i]);
  }
  
  // synchronously start music
  alSourcePlayv(4, musicSources);
  
  // hint sounds
  alGenSources(2, hintSources);
  alGenBuffers(2, hintBuffers);
  hintBuffers[0] = alutCreateBufferFromFile("../res/bling_low.wav");
  hintBuffers[1] = alutCreateBufferFromFile("../res/bling_high.wav");
  alSourcei(hintSources[0], AL_BUFFER, hintBuffers[0]);
  alSourcei(hintSources[1], AL_BUFFER, hintBuffers[1]);
  alSource3f(hintSources[0], AL_POSITION,  -0.1, 0.0, 0.0);
  alSource3f(hintSources[1], AL_POSITION,  0.1, 0.0, 0.0);
  alSourcef(hintSources[0], AL_GAIN, 1.0 );
  alSourcef(hintSources[1], AL_GAIN, 0.4 );
}

void BoxAlAudible::auralize( ::view::AlRenderer& renderer )
{
  model::Tube::control_point_type  tube_bounds;
  try
  { tube_bounds = _box->tube_object()->getVerticalBoundsAt( renderer.game_model()->timestamp() ); }
  catch( std::out_of_range e )
  {
    std::cerr << "flappy_box::controller::BoxObjectLogic::advance: Unable to determine collision bounds: " << e.what() << std::endl;
    return;
  }

  double timestep_sec = renderer.game_model()->timestep().count();

  ALfloat pos = _box->position().z();
  
  // calculate sound position as normalized parameter inbetween [0,1] ~ [lowerBound,upperBound]
  double rel_pos = (pos - tube_bounds.first) / (tube_bounds.second - tube_bounds.first); 
  rel_pos = std::min( 1., std::max( 0., rel_pos ) ); 

  std::clog << "BoxAlAudible::auralize: pos " << pos << " rel " << rel_pos << "." << std::endl;

  // hint sound
  static double interval = 0.0;
  interval += timestep_sec;
  double threshold = 1.0; // time threshold in seconds, defines heart beat of hint sound

  if ( interval > threshold )
  {
    // value of next hint sound (1 second in future)

      model::Tube::control_point_type target_tube_bounds;
    try
    { target_tube_bounds = _box->tube_object()->getVerticalBoundsAt( renderer.game_model()->timestamp() + std::chrono::seconds( 2 ) ); }
    catch( std::out_of_range e )
    {
      std::cerr << "flappy_box::controller::BoxAlAudible::auralize: Unable to determine collision bounds: " << e.what() << std::endl;
      return;
    }

    double target_width = target_tube_bounds.second - target_tube_bounds.first;
    double target_mid = .5 * ( target_tube_bounds.first + target_tube_bounds.second );

    // only play hint when distance is too far from optimum (let's say 20%)
    if( fabs( target_mid - pos) > 0.2 * target_width )
    {
      // higher or lower than current pos
      if ( pos < target_mid )
        alSourcePlay( hintSources[0] );
      else
        alSourcePlay( hintSources[1] );
    }

    interval = 0; 
  }

  // low/high sounds
  alSourcef(musicSources[0], AL_GAIN, 1.0 - rel_pos);
  alSourcef(musicSources[1], AL_GAIN, rel_pos);
  
  // beat
  double beat_gain = pow(1.0 - 2 * fabs(0.5 - rel_pos),2.0);
  beat_gain = std::min( 1., std::max( 0., beat_gain ) );
  alSourcef(musicSources[2], AL_GAIN, beat_gain );
  
  // synth
  alSourcef(musicSources[3], AL_GAIN, beat_gain * beat_gain );
  alSource3f(musicSources[3], AL_POSITION, 2*rel_pos - 1.0 , 0.0, 0.0 );
  
  // white noise
  double player_gain = pow(2 * fabs(0.5 - rel_pos),10.0);
  player_gain = std::min( 1., std::max( 0., player_gain ) );
  alSourcef(whiteNoiseSource, AL_GAIN, 0.1 * player_gain );
  alSource3f(whiteNoiseSource, AL_POSITION, 2*rel_pos - 1.0 , 0.0, 0.0 );
}
