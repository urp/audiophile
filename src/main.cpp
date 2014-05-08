# include "controller/default_engine.hpp"
# include "model/game.hpp"

# include <GL/glut.h>

int main( int argc, char** argv )
{
  std::shared_ptr< audiophile::controller::Engine > game_engine( new audiophile::controller::DefaultEngine() );

  game_engine->init( argc, argv );
  
  game_engine->run();

  return 0;
}
