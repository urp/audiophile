# pragma once

# include "view/gl/headers.hpp"

# include <iostream>

using namespace std;

inline void exitOnGLError(const char* error_message)
{
  const GLenum error_value = glGetError();
  if( error_value != GL_NO_ERROR)
  {
    cerr << error_message << ": errno " << error_value << " " << string( reinterpret_cast< const char*>( gluErrorString( error_value) ) ) << endl;
    exit(1);
  }
}
