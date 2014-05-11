# include "flappy_box/view/box_gl_drawable.hpp"

# include <GL/gl.h>
# include <GL/glu.h>

using namespace ::flappy_box::view;


BoxGlDrawable::BoxGlDrawable(const std::shared_ptr< model::Box >& b )
: _model( b )
{}


void BoxGlDrawable::visualize( ::view::GlRenderer&, ::view::GlutWindow& )
{
  glPushMatrix();
  {
    std::chrono::milliseconds rotation_interval( 5000 );
    double angle = _model->angle();
    const vec3_type pos = _model->position();
    glTranslated( pos[0], pos[1], pos[2] );
    glRotated( angle, 0., 0., 1. );
    glColor3f( .5,.5,.5 );

    float box_vertices[] = { -1.,-1.,-1.
                           , -1.,-1., 1.
                           , -1., 1.,-1.
                           , -1., 1., 1.
                           ,  1.,-1.,-1.
                           ,  1.,-1., 1.
                           ,  1., 1.,-1.
                           ,  1., 1., 1.
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
  glPopMatrix();
}
