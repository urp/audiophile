# include "flappy_box/controller/flappy_engine.hpp"

# include "model/game.hpp"

int main( int argc, char** argv )
{
  std::shared_ptr< ::model::Game > game_model( new ::model::Game );
  std::shared_ptr< ::controller::Logic > game_logic( new ::controller::Logic( game_model ) );

  std::shared_ptr< ::controller::Engine > game_engine( new ::flappy_box::controller::FlappyEngine( game_logic ) );

  game_engine->init( argc, argv );
  game_engine->run();

  return 0;
}
