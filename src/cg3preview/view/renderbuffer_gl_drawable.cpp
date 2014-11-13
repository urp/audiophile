# include "cg3preview/view/renderbuffer_gl_drawable.hpp"

# include "view/gl/shader_program.hpp"

# include "view/gl/wrap/gl_type.hpp"

# include "view/gl/utilities.hpp"

using namespace ::cg3preview::view;
using namespace ::std;

RenderbufferGlDrawable::RenderbufferGlDrawable( shared_ptr< model::Renderbuffer > const& rb )
: _renderbuffer( rb ), _quad_vbo(0), _quad_vao(0)
{
  {
    GLfloat quad_data[] = { -1., -1., 1. //v0
                          ,  0.,  1., 1. //t0
                          ,  1., -1., 1. //v1
                          ,  1.,  1., 1. //t1
                          , -1.,  1., 1. //v2
                          ,  0.,  0., 1. //t2
                          ,  1.,  1., 1. //v3
                          ,  1.,  0., 1. //t3
                          };
    glGenBuffers( 1, &_quad_vbo );
    glBindBuffer( GL_ARRAY_BUFFER, _quad_vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof(quad_data), quad_data, GL_STATIC_DRAW);

    exitOnGLError( "Error setting up VBO" );
  }

  {
    glGenVertexArrays( 1, &_quad_vao );
    glBindVertexArray( _quad_vao );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0 );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray( 0 );
    glEnableVertexAttribArray( 1 );

    exitOnGLError( "Error setting up VAO" );
  }

  {
    glGenTextures( 1, &_composed_image_texture );
    glBindTexture( GL_TEXTURE_2D, _composed_image_texture );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

    // upload a white image as default.
    auto const image_extent = rb->get_render_manager()->get_resolution();
    vector< ::view::gl::gl_type< Tile::pixel_format_type::value_type >::type > default_image( image_extent[0] * image_extent[1] * 3, 255 );
    glTexImage2D( GL_TEXTURE_2D
                , 0
                , GL_RGB, image_extent[0], image_extent[1]
                , 0, GL_RGB
                , ::view::gl::gl_type< Tile::pixel_format_type::value_type >::gl_enum
                , default_image.data() );

    exitOnGLError( "Error setting up composed image texture" );
  }

  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  glBindVertexArray( 0 );
  glBindTexture( GL_TEXTURE_2D, 0 );
}

ShaderProgram setupShaderProgram()
{
  Shader vertex_shader( GL_VERTEX_SHADER );
  vertex_shader.loadFromFile( "../res/shaders/texture_lookup.vert" );
  vertex_shader.compile();

  Shader fragment_shader( GL_FRAGMENT_SHADER );
  fragment_shader.loadFromFile( "../res/shaders/texture_lookup.frag" );
  fragment_shader.compile();

  ShaderProgram prog;
  prog.attachShader( vertex_shader );
  prog.attachShader( fragment_shader );
  prog.link();

  return prog;
}

void RenderbufferGlDrawable::visualize( ::view::GlRenderer& r, ::view::GlutWindow& )
{
  // update texture
  if( _renderbuffer->has_updates() )
  {
    Tile t = _renderbuffer->pop_updated_tile();
    for( auto& pixel : t.image )
      pixel /= t.weight;

    glBindTexture( GL_TEXTURE_2D, _composed_image_texture );
    glTexSubImage2D( GL_TEXTURE_2D, 0
                   , t.origin[0], t.origin[1], t.image.w(), t.image.h()
                   , GL_RGB
                   , ::view::gl::gl_type< Tile::pixel_format_type::value_type >::gl_enum
                   , t.image.begin() );
  }

  static ShaderProgram prog = setupShaderProgram();
  prog.use();

  glUniform1i( 3, 0 );
  glActiveTexture( GL_TEXTURE0 + 0 );
  glBindTexture( GL_TEXTURE_2D, _composed_image_texture );

  glBindVertexArray( _quad_vao );

  glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

  prog.disable();

  exitOnGLError( "Error in RenderbufferGlDrawable::visualize");
}
