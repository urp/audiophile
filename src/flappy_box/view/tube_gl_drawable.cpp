# include "flappy_box/view/tube_gl_drawable.hpp"

# include "flappy_box/model/box.hpp"

# include "view/gl_shader.hpp"
# include "view/gl_shader_program.hpp"

# include <random>

using namespace ::flappy_box::view;
using namespace std;
using namespace chrono;

TubeGlDrawable::TubeGlDrawable( shared_ptr< model::Tube > const& t )
: _model( t )
, _first_timestamp( steady_clock::time_point::min() )
, _cp_bounds( steady_clock::time_point::min(), steady_clock::time_point::min() )
{
  glGenBuffers( 2, lineVBOs );
}


void TubeGlDrawable::visualize( ::view::GlRenderer& r, ::view::GlutWindow& )
{
  glPushMatrix();
  glEnableClientState( GL_VERTEX_ARRAY );
  {
    auto const& cps = _model->getControlPoints();

    if( not cps.empty() )
    {
      steady_clock::time_point timestamp = r.game_model()->timestamp();

      if( _first_timestamp == steady_clock::time_point::min() )
        _first_timestamp = timestamp;

      auto current_cp_bounds = make_pair( cps.begin()->first, cps.rbegin()->first );
      if(  _cp_bounds != current_cp_bounds )
      {
        auto const old_bounds = _cp_bounds;
        _cp_bounds = current_cp_bounds;

        size_t reused_cps = 0;
        auto it_old_end = cps.find( old_bounds.second );
        if( it_old_end == cps.end() ) 
          it_old_end = cps.begin();
        else
          reused_cps = distance( cps.begin(), it_old_end );

        clog << "TubeGlDrawable::visualize: Reusing " << reused_cps << " control points." << endl;

        updateWallSurface( reused_cps, it_old_end );
        //updateWallPoints( reused_cps, it_old_end );
        updateLines( reused_cps, it_old_end );
      }

      glTranslated( duration_cast< duration<double> >( _first_timestamp - timestamp ).count(), 0., 0. );

      // Draw primitives
      drawWallSurfaceWithShaders( duration_cast< duration<double> >( timestamp - _first_timestamp ) );
      //drawWallPoints();
      drawLines();

      // Unbind buffer
      glBindBuffer( GL_ARRAY_BUFFER, 0 );

    } // ( not cps.empty() )
  }
  glDisableClientState( GL_VERTEX_ARRAY );
  glPopMatrix();
}

void TubeGlDrawable::updateLines( size_t reused_cps, flappy_box::model::Tube::control_point_map_type::const_iterator const& it_old_end )
{
  copy( lower_vertices.end() - ( reused_cps ? reused_cps+1 : 0 ), lower_vertices.end(), lower_vertices.begin() );
  copy( upper_vertices.end() - ( reused_cps ? reused_cps+1 : 0 ), upper_vertices.end(), upper_vertices.begin() );

  auto const& cps = _model->getControlPoints();

  lower_vertices.resize( cps.size() );
  upper_vertices.resize( cps.size() );
  
  size_t new_vertex_idx = reused_cps;
  
  for( auto cp_it = it_old_end; cp_it != cps.end(); ++cp_it )
  {
    double x_coord = duration_cast< duration<double> >( cp_it->first - _first_timestamp ).count();
    //clog << "TubeGlDrawable::visualize: add vertex x " << x_coord << " lower " << tcp.second. first  << " upper " << tcp.second.second << endl;
    lower_vertices[ new_vertex_idx ] = { x_coord, 0., cp_it->second.first };
    upper_vertices[ new_vertex_idx ] = { x_coord, 0., cp_it->second.second };
    new_vertex_idx++;
  }
  
  /// Specify lower and upper line vertices
  glBindBuffer( GL_ARRAY_BUFFER, lineVBOs[0] );
  glBufferData( GL_ARRAY_BUFFER, lower_vertices.size() * 3 * sizeof(double) , lower_vertices.data(), GL_DYNAMIC_DRAW );
  glBindBuffer( GL_ARRAY_BUFFER, lineVBOs[1] );
  glBufferData( GL_ARRAY_BUFFER, upper_vertices.size() * 3 * sizeof(double) , upper_vertices.data(), GL_DYNAMIC_DRAW );
}

void TubeGlDrawable::drawLines()
{
  glColor3f( .1,.7,.2 );
  //Make the new VBO active
  glBindBuffer( GL_ARRAY_BUFFER, lineVBOs[0] );
  glVertexPointer( 3, GL_DOUBLE, 0, nullptr );
  glDrawArrays( GL_LINE_STRIP, 0, lower_vertices.size() );

  glColor3f( .1,.2,.7 );
  //Make the new VBO 1 active
  glBindBuffer( GL_ARRAY_BUFFER, lineVBOs[1] );
  glVertexPointer( 3, GL_DOUBLE, 0, nullptr );
  glDrawArrays( GL_LINE_STRIP, 0, upper_vertices.size() );
}

void TubeGlDrawable::updateWallSurface( size_t reused_cps, flappy_box::model::Tube::control_point_map_type::const_iterator const& it_old_end )
{
  auto const& cps = _model->getControlPoints();

  // vertices

  if( not s_segment_vbos.empty() )
  {
    glDeleteBuffers( s_segment_vbos.size() - reused_cps, s_segment_vbos.data() );
    copy( s_segment_vbos.end() - reused_cps, s_segment_vbos.end(), s_segment_vbos.begin() );
  }
  s_segment_vbos.resize( cps.size() - 1 );
  glGenBuffers( s_segment_vbos.size() - reused_cps, &s_segment_vbos[ reused_cps ] );

  copy( s_segment_samples.end() - reused_cps, s_segment_samples.end(), s_segment_samples.begin() );
  s_segment_samples.resize( s_segment_vbos.size() );

  // normals

  if( not s_normal_vbos.empty() )
  { 
    glDeleteBuffers( s_normal_vbos.size() - reused_cps, s_normal_vbos.data() );
    copy( s_normal_vbos.end() - reused_cps, s_normal_vbos.end(), s_normal_vbos.begin() );
  }
  s_normal_vbos.resize( cps.size() - 1 );
  glGenBuffers( s_normal_vbos.size() - reused_cps, &s_normal_vbos[ reused_cps ] );

  copy( s_normal_samples.end() - reused_cps, s_normal_samples.end(), s_normal_samples.begin() );
  s_normal_samples.resize( s_normal_vbos.size() );

  size_t seg_idx = reused_cps;
  for( auto cp_it = it_old_end; cp_it != prev( cps.end() ) ; ++cp_it )
  {
    auto const& tcp1 = *cp_it;
    auto const& tcp2 = *next( cp_it );
    constexpr size_t samples_per_segment = 60;

    s_segment_samples[ seg_idx ].resize( samples_per_segment );
    s_normal_samples[ seg_idx ].resize( samples_per_segment );

    double const t1 = duration_cast< duration<double> >( tcp1.first - _first_timestamp ).count();
    double const t2 = duration_cast< duration<double> >( tcp2.first - _first_timestamp ).count();
    double const center1 = ( tcp1.second.second + tcp1.second.first ) * .5;
    double const center2 = ( tcp2.second.second + tcp2.second.first ) * .5;
    double const radius1 = ( tcp1.second.second - tcp1.second.first ) * .5;
    double const radius2 = ( tcp2.second.second - tcp2.second.first ) * .5;

    for( size_t s_idx = 0; s_idx < samples_per_segment; s_idx+=2 )
    {
      double const angle = M_PI * s_idx / ( samples_per_segment-2. );
      double const sin_a = sin( angle );
      double const cos_a = cos( angle );

      s_segment_samples[ seg_idx ][ s_idx   ] = { t1, radius1 * sin_a, radius1 * cos_a + center1 };
      s_segment_samples[ seg_idx ][ s_idx+1 ] = { t2, radius2 * sin_a, radius2 * cos_a + center2 };

      double const h1 = radius1 * cos_a + center1;
      double const h2 = radius2 * cos_a + center2;
      double n_x = ( h1 - h2 ) * -cos_a;
      n_x /= hypot( n_x, t2 -t1 );
      double n_y = -sin_a * (1-n_x*n_x);
      double n_z = -cos_a * (1-n_x*n_x);
      
      //clog << "normal seg " << seg_idx << " norm " << s_idx << " x " << n_x << " y " << n_y << " z " << n_z << endl;
      
      s_normal_samples[ seg_idx ][ s_idx   ] = { n_x, n_y, n_z };
      s_normal_samples[ seg_idx ][ s_idx+1 ] = { n_x, n_y, n_z };
    }

    glBindBuffer( GL_ARRAY_BUFFER, s_segment_vbos[seg_idx] );
    glBufferData( GL_ARRAY_BUFFER, s_segment_samples[seg_idx].size() * 3 * sizeof(double) , s_segment_samples[seg_idx].data(), GL_STATIC_DRAW);

    glBindBuffer( GL_ARRAY_BUFFER, s_normal_vbos[seg_idx] );
    glBufferData( GL_ARRAY_BUFFER, s_normal_samples[seg_idx].size() * 3 * sizeof(double) , s_normal_samples[seg_idx].data(), GL_STATIC_DRAW);

    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    seg_idx++;
  }
}

void TubeGlDrawable::drawWallSurface( chrono::duration< double > const& t )
{
  glPolygonMode( GL_FRONT, GL_FILL );
  glColor3f( .6,.6,.6 );
  //glCullFace( GL_FRONT_AND_BACK );

  glEnable( GL_LIGHTING );
  glEnable( GL_LIGHT0 );
  float l0_pos[4] = { t.count(), 0.1, _model->getBox()->position().z(), 1. };
  glLightfv( GL_LIGHT0, GL_POSITION, l0_pos );
  glLightf (GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 1. );
  glLightf (GL_LIGHT0, GL_QUADRATIC_ATTENUATION, .5 );

  glEnable( GL_COLOR_MATERIAL );
  float mat_ambient[4] = { 0.1, 0.2, 0.2, 1. };
  glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient );
  float mat_diffuse[4] = { 0.7, 0.6, 0.6, 1. };
  glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse );
  float mat_specular[4] = { 0.3, 0.3, 0.1, 1. };
  glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular );
  float mat_shininess[] = { 10. };
  glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess );

  glShadeModel( GL_SMOOTH );

  glFrontFace( GL_CCW );

  glEnableClientState( GL_NORMAL_ARRAY );
  for( size_t seg_idx = 0; seg_idx < s_segment_vbos.size(); seg_idx++ )
  {
    glBindBuffer( GL_ARRAY_BUFFER, s_segment_vbos[seg_idx] );
    glVertexPointer( 3, GL_DOUBLE, 0, nullptr );

    glBindBuffer( GL_ARRAY_BUFFER, s_normal_vbos[seg_idx] );
    glNormalPointer( GL_DOUBLE, 0, nullptr );

    glDrawArrays( GL_TRIANGLE_STRIP, 0, s_segment_samples[seg_idx].size() );
  }
  glDisableClientState( GL_NORMAL_ARRAY );

  glDisable( GL_LIGHT0 );
  glDisable( GL_LIGHTING );
  
}

void exitOnGLError(const char* error_message)
{
  const GLenum error_value = glGetError();
  if( error_value != GL_NO_ERROR)
  {
    cerr << error_message << ": errno " << error_value << " " << string( reinterpret_cast< const char*>( gluErrorString( error_value) ) ) << endl;
    std::abort();
  }
}

ShaderProgram setupWallSurfaceShaderProgram()
{
  Shader vertex_shader( GL_VERTEX_SHADER );
  vertex_shader.loadFromFile( "../res/shaders/blinn_phong.vert" );
  vertex_shader.compile();

  Shader fragment_shader( GL_FRAGMENT_SHADER );
  fragment_shader.loadFromFile( "../res/shaders/blinn_phong.frag" );
  fragment_shader.compile();

  ShaderProgram prog;
  prog.attachShader( vertex_shader );
  prog.attachShader( fragment_shader );
  prog.link();

  return prog;
}


struct MatrixBlock
{
  Eigen::Matrix< GLfloat, 4, 4 > pvm_matrix;
  Eigen::Matrix< GLfloat, 4, 4 >  vm_matrix;
  Eigen::Matrix< GLfloat, 4, 4 >   n_matrix;
};

struct LightBlock
{
  GLfloat l_pos[4];
  GLfloat constant_attentuation;
  GLfloat linear_attentuation;
  GLfloat quadratic_attentuation;
};

struct MaterialBlock
{
  GLfloat diffuse[4];
  GLfloat ambient[4];
  GLfloat specular[4];
  GLfloat shininess;
};


void TubeGlDrawable::drawWallSurfaceWithShaders( chrono::duration< double > const& t )
{

  static ShaderProgram prog = setupWallSurfaceShaderProgram();
  prog.use();
  {
    { 
      MatrixBlock mat_block;
      glGetFloatv( GL_MODELVIEW_MATRIX, mat_block.vm_matrix.data() );

      Eigen::Matrix< GLfloat, 4, 4 > p_matrix;
      glGetFloatv( GL_PROJECTION_MATRIX, p_matrix.data() );

      mat_block.pvm_matrix = p_matrix * mat_block.vm_matrix;
      mat_block.n_matrix = mat_block.vm_matrix/*.block(0,0,3,3)*/.inverse().transpose();

      const GLuint binding_point = 0;
      prog.bindUniformBlock( "Matrices", binding_point );

      GLuint mat_block_ubo;
      glGenBuffers( 1, &mat_block_ubo );
      glBindBuffer( GL_UNIFORM_BUFFER, mat_block_ubo );
      glBufferData( GL_UNIFORM_BUFFER, sizeof(MatrixBlock), &mat_block, GL_DYNAMIC_DRAW );
      glBindBufferBase( GL_UNIFORM_BUFFER, binding_point, mat_block_ubo );

      exitOnGLError( "Binding Matrices uniform block");
    }

    {
      LightBlock light_block = { { t.count(), 0.1, _model->getBox()->position().z(), 1. }
                               , 1., .0 , .1 
                               };

      GLuint binding_point = 1;
      prog.bindUniformBlock( "Light", binding_point );

      GLuint light_block_ubo;
      glGenBuffers( 1, &light_block_ubo );
      glBindBuffer( GL_UNIFORM_BUFFER, light_block_ubo );
      glBufferData( GL_UNIFORM_BUFFER, sizeof( LightBlock ), &light_block, GL_DYNAMIC_DRAW );
      glBindBufferBase( GL_UNIFORM_BUFFER, binding_point, light_block_ubo );

      exitOnGLError( "Binding Lights uniform block");
    }

    {
      MaterialBlock material_block ={ { 0.7, 0.9, 0.9, 1. }, { 0., 0., 0., 1. }, { 0.4, 0.1, 0.1, 1. }, 10. };

      GLuint binding_point = 2;
      prog.bindUniformBlock( "Materials", binding_point );

      GLuint material_block_ubo;
      glGenBuffers( 1, &material_block_ubo );
      glBindBuffer( GL_UNIFORM_BUFFER, material_block_ubo );
      glBufferData( GL_UNIFORM_BUFFER, sizeof(MaterialBlock), &material_block, GL_DYNAMIC_DRAW );
      glBindBufferBase( GL_UNIFORM_BUFFER, binding_point, material_block_ubo );

      exitOnGLError( "Binding Material uniform block");
    }

    {
      glPolygonMode( GL_FRONT, GL_FILL );
      glCullFace( GL_FRONT );
      glFrontFace( GL_CCW );
      glShadeModel( GL_SMOOTH );
    }

    for( size_t seg_idx = 0; seg_idx < s_segment_vbos.size(); seg_idx++ )
    {
      glBindBuffer( GL_ARRAY_BUFFER, s_segment_vbos[seg_idx] );
      GLint position_loc = glGetAttribLocation( prog.id(), "position" );
      glEnableVertexAttribArray( position_loc );// 1rst attribute buffer : vertices
      glVertexAttribPointer( position_loc
                           , 3         // size
                           , GL_DOUBLE // type
                           , GL_FALSE  // normalized?
                           , 0         // stride
                           , nullptr    // array buffer offset
                           );
      exitOnGLError( "Setting glEnableVertexAttribPointer(vertices)" );

      glBindBuffer( GL_ARRAY_BUFFER, s_normal_vbos[seg_idx] );
      GLint normal_loc = glGetAttribLocation( prog.id(), "normal" );
      glEnableVertexAttribArray( normal_loc );// 2nd attribute buffer : normals
      glVertexAttribPointer( normal_loc
                           , 3         // size
                           , GL_DOUBLE // type
                           , GL_FALSE  // normalized?
                           , 0         // stride
                           , nullptr   // array buffer offset
                           );
      exitOnGLError( "Setting glEnableVertexAttribPointer(normals)" );

      glDrawArrays( GL_TRIANGLE_STRIP, 0, s_segment_samples[seg_idx].size() );
      glDisableVertexAttribArray( position_loc );
      glDisableVertexAttribArray( normal_loc );
    }

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
  }
  prog.disable();

}
