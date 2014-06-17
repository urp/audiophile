# include "flappy_box/controller/flappy_engine.hpp"

int main( int argc, char** argv )
{
  std::shared_ptr< ::controller::Engine > game_engine( new ::flappy_box::controller::FlappyEngine );

  game_engine->init( argc, argv );
  game_engine->run();

  return 0;
}
