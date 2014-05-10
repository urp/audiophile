# include "controller/glut_engine.hpp"
#include <controller/default_logic.hpp>
# include "model/game.hpp"

using namespace audiophile;

int main( int argc, char** argv )
{
  std::shared_ptr< model::Game > game_model( new model::Game );
  std::shared_ptr< controller::Logic > game_logic( new controller::DefaultLogic( game_model ) );
  
  std::shared_ptr< controller::Engine > game_engine( new controller::GlutEngine( game_logic ) );

  game_engine->init( argc, argv );

  game_engine->run();

  return 0;
}
