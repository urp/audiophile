# include "view/default_gl_drawable.hpp"

# include <GL/gl.h>
# include <GL/glu.h>

using namespace audiophile::view;

DefaultGLDrawable::DefaultGLDrawable( const std::shared_ptr< model::GameObject >& o )
: _model( o )
{}


void DefaultGLDrawable::glDraw( GLRenderer&, GLUTWindow& )
{
  std::clog << "Hi from DefaultDrawable::glDraw." << std::endl; 
  //Clear information from last draw
  glBegin(GL_QUADS); //Begin quadrilateral coordinates
  //Trapezoid
  glVertex3f(-0.7f, -1.5f, -5.0f);
  glVertex3f(0.7f, -1.5f, -5.0f);
  glVertex3f(0.7f, -0.5f, -5.0f);
  glVertex3f(-0.7f, -0.5f, -5.0f);
  glEnd(); //End quadrilateral coordinates
  glBegin(GL_TRIANGLES); //Begin triangle coordinates
  //Triangle
  glVertex3f(-0.5f, 0.5f, -5.0f);
  glVertex3f(-1.0f, 1.5f, -5.0f);
  glVertex3f(-1.5f, 0.5f, -5.0f);
  glEnd(); //End triangle coordinates

}
