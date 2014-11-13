# include "cg3preview/controller/engine.hpp"

int main( int argc, char** argv )
{
  std::shared_ptr< ::controller::Engine > game_engine( new ::cg3preview::controller::Engine );

  game_engine->init( argc, argv );
  game_engine->run();

  return 0;
}
