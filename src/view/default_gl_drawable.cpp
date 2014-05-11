# include "view/default_gl_drawable.hpp"

# include <GL/gl.h>
# include <GL/glu.h>

using namespace audiophile::view;

DefaultGLDrawable::DefaultGLDrawable( const std::shared_ptr< model::GameObject >& o )
: _model( std::dynamic_pointer_cast< model::DefaultGameObject >( o ) )
{
  
  if( not _model )
    throw std::logic_error( "DefaultGLDrawable::DefaultGLDrawable: GameObject is no DefaultGameObject." );
}


void DefaultGLDrawable::glDraw( GlRenderer&, GlutWindow& )
{
  std::clog << "Hi from DefaultDrawable::glDraw." << std::endl; 
  //Clear information from last draw
//   glBegin(GL_QUADS); //Begin quadrilateral coordinates
//   //Trapezoid
//   glVertex3f(-0.7f, -1.5f, -5.0f);
//   glVertex3f(0.7f, -1.5f, -5.0f);
//   glVertex3f(0.7f, -0.5f, -5.0f);
//   glVertex3f(-0.7f, -0.5f, -5.0f);
//   glEnd(); //End quadrilateral coordinates
//   glBegin(GL_TRIANGLES); //Begin triangle coordinates
//   //Triangle
//   glVertex3f(-0.5f, 0.5f, -5.0f);
//   glVertex3f(-1.0f, 1.5f, -5.0f);
//   glVertex3f(-1.5f, 0.5f, -5.0f);
//   glEnd(); //End triangle coordinates

  glClearColor( 0.1, 0.2, 0.3, 1.);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt( 0,-7,0, 0,0,0, 0,0,1 );
  {
    std::chrono::milliseconds rotation_interval( 5000 );
    double angle = _model->angle();
    
    glRotated( angle, 0., 0., 1. );
    glColor3f( .5,.5,.5 );
    
    float box_vertices[] = { -1.,-1.,-1.
      , -1.,-1., 1.
      , -1., 1.,-1.
      , -1., 1., 1.
      , 1.,-1.,-1.
      , 1.,-1., 1.
      , 1., 1.,-1.
      , 1., 1., 1.
    };
    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer( 3, GL_FLOAT, 0, box_vertices );
    //glColorPointer( );
    unsigned short idx_wire_loop1[] = { 2,6,7,3,1,5,7,6,4,0 };
    glDrawElements( GL_LINE_LOOP, 10, GL_UNSIGNED_SHORT, idx_wire_loop1 );
    
    unsigned short idx_wire_lines[] = { 2,3, 0,1, 4,5 };
    glDrawElements( GL_LINES, 6, GL_UNSIGNED_SHORT, idx_wire_lines );
    glDisableClientState( GL_VERTEX_ARRAY );
  }
}
