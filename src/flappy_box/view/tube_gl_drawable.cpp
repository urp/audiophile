# include "flappy_box/view/tube_gl_drawable.hpp"

# include "flappy_box/model/box.hpp"

# include <random>

using namespace ::flappy_box::view;
using namespace std::chrono;

TubeGlDrawable::TubeGlDrawable( std::shared_ptr< model::Tube > const& t )
: _model( t )
, _first_timestamp( steady_clock::time_point::min() )
, _cp_bounds( steady_clock::time_point::min(), steady_clock::time_point::min() )
{
  //Create a new VBO and use the variable id to store the VBO id
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

      auto current_cp_bounds = std::make_pair( cps.begin()->first, cps.rbegin()->first );
      if(  _cp_bounds != current_cp_bounds )
      {
        auto const old_bounds = _cp_bounds;
        _cp_bounds = current_cp_bounds;

        std::size_t reused_cps = 0;
        auto it_old_end = cps.find( old_bounds.second );
        if( it_old_end == cps.end() ) 
          it_old_end = cps.begin();
        else
          reused_cps = std::distance( cps.begin(), it_old_end );

        std::clog << "TubeGlDrawable::visualize: Reusing " << reused_cps << " control points." << std::endl;

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

void TubeGlDrawable::updateLines( std::size_t reused_cps, flappy_box::model::Tube::control_point_map_type::const_iterator const& it_old_end )
{
  std::copy( lower_vertices.end() - ( reused_cps ? reused_cps+1 : 0 ), lower_vertices.end(), lower_vertices.begin() );
  std::copy( upper_vertices.end() - ( reused_cps ? reused_cps+1 : 0 ), upper_vertices.end(), upper_vertices.begin() );

  auto const& cps = _model->getControlPoints();

  lower_vertices.resize( cps.size() );
  upper_vertices.resize( cps.size() );
  
  std::size_t new_vertex_idx = reused_cps;
  
  for( auto cp_it = it_old_end; cp_it != cps.end(); ++cp_it )
  {
    double x_coord = duration_cast< duration<double> >( cp_it->first - _first_timestamp ).count();
    //std::clog << "TubeGlDrawable::visualize: add vertex x " << x_coord << " lower " << tcp.second. first  << " upper " << tcp.second.second << std::endl;
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


void TubeGlDrawable::updateWallPoints( std::size_t reused_cps, flappy_box::model::Tube::control_point_map_type::const_iterator const& it_old_end )
{
  static std::default_random_engine generator;
  static std::uniform_real_distribution< double > distribution( 0., 1. );

  auto const& cps = _model->getControlPoints();
  
  glDeleteBuffers( p_segment_vbos.size() - reused_cps, p_segment_vbos.data() );
  std::copy( p_segment_vbos.end() - reused_cps, p_segment_vbos.end(), p_segment_vbos.begin() );
  p_segment_vbos.resize( _model->getControlPoints().size() - 1 );
  glGenBuffers( p_segment_vbos.size() - reused_cps, &p_segment_vbos[ reused_cps ] );

  std::copy( p_segment_samples.end() - reused_cps, p_segment_samples.end(), p_segment_samples.begin() );
  p_segment_samples.resize( p_segment_vbos.size() );

  std::size_t seg_idx = reused_cps;
  for( auto cp_it = it_old_end; cp_it != std::prev( cps.end() ) ; ++cp_it )
  {
    auto const& tcp1 = *cp_it;
    auto const& tcp2 = *std::next( cp_it );
    constexpr std::size_t num_p_segment_samples = 200;
    p_segment_samples[ seg_idx ].resize( num_p_segment_samples );

    double const t1 = duration_cast< duration<double> >( tcp1.first - _first_timestamp ).count();
    double const t2 = duration_cast< duration<double> >( tcp2.first - _first_timestamp ).count();
    double const center1 = ( tcp1.second.second + tcp1.second.first ) * .5;
    double const center2 = ( tcp2.second.second + tcp2.second.first ) * .5;
    double const radius1 = ( tcp1.second.second - tcp1.second.first ) * .5;
    double const radius2 = ( tcp2.second.second - tcp2.second.first ) * .5;

    for( std::size_t s_idx = 0; s_idx < num_p_segment_samples; ++s_idx )
    {
      double const angle = M_PI * distribution( generator );
      double const ipol_factor = distribution( generator );

      double const x_coord =     t1 * ( 1. - ipol_factor ) +      t2 * ipol_factor;
      double const radius = radius1 * ( 1. - ipol_factor ) + radius2 * ipol_factor;
      double const center = center1 * ( 1. - ipol_factor ) + center2 * ipol_factor;
      p_segment_samples[ seg_idx ][ s_idx ] = { x_coord
                                              , radius * sin( angle )
                                              , radius * cos( angle ) + center
                                              };
    }

    glBindBuffer( GL_ARRAY_BUFFER, p_segment_vbos[seg_idx] );
    glBufferData( GL_ARRAY_BUFFER, p_segment_samples[seg_idx].size() * 3 * sizeof(double), p_segment_samples[seg_idx].data(), GL_STATIC_DRAW);

    seg_idx++;
  }
}

void TubeGlDrawable::drawWallPoints()
{
  glPointSize( 2. );
  glColor3f( .7,.7,.7 );
  for( std::size_t seg_idx = 0; seg_idx < p_segment_vbos.size(); seg_idx++ )
  {
    glBindBuffer( GL_ARRAY_BUFFER, p_segment_vbos[seg_idx] );
    glVertexPointer( 3, GL_DOUBLE, 0, nullptr );
    glDrawArrays( GL_POINTS, 0, p_segment_samples[seg_idx].size() );
  }
}


void TubeGlDrawable::updateWallSurface( std::size_t reused_cps, flappy_box::model::Tube::control_point_map_type::const_iterator const& it_old_end )
{
  auto const& cps = _model->getControlPoints();

  // vertices

  if( not s_segment_vbos.empty() )
  {
    glDeleteBuffers( s_segment_vbos.size() - reused_cps, s_segment_vbos.data() );
    std::copy( s_segment_vbos.end() - reused_cps, s_segment_vbos.end(), s_segment_vbos.begin() );
  }
  s_segment_vbos.resize( cps.size() - 1 );
  glGenBuffers( s_segment_vbos.size() - reused_cps, &s_segment_vbos[ reused_cps ] );

  std::copy( s_segment_samples.end() - reused_cps, s_segment_samples.end(), s_segment_samples.begin() );
  s_segment_samples.resize( s_segment_vbos.size() );

  // normals

  if( not s_normal_vbos.empty() )
  { 
    glDeleteBuffers( s_normal_vbos.size() - reused_cps, s_normal_vbos.data() );
    std::copy( s_normal_vbos.end() - reused_cps, s_normal_vbos.end(), s_normal_vbos.begin() );
  }
  s_normal_vbos.resize( cps.size() - 1 );
  glGenBuffers( s_normal_vbos.size() - reused_cps, &s_normal_vbos[ reused_cps ] );

  std::copy( s_normal_samples.end() - reused_cps, s_normal_samples.end(), s_normal_samples.begin() );
  s_normal_samples.resize( s_normal_vbos.size() );

  std::size_t seg_idx = reused_cps;
  for( auto cp_it = it_old_end; cp_it != std::prev( cps.end() ) ; ++cp_it )
  {
    auto const& tcp1 = *cp_it;
    auto const& tcp2 = *std::next( cp_it );
    constexpr std::size_t samples_per_segment = 60;

    s_segment_samples[ seg_idx ].resize( samples_per_segment );
    s_normal_samples[ seg_idx ].resize( samples_per_segment );

    double const t1 = duration_cast< duration<double> >( tcp1.first - _first_timestamp ).count();
    double const t2 = duration_cast< duration<double> >( tcp2.first - _first_timestamp ).count();
    double const center1 = ( tcp1.second.second + tcp1.second.first ) * .5;
    double const center2 = ( tcp2.second.second + tcp2.second.first ) * .5;
    double const radius1 = ( tcp1.second.second - tcp1.second.first ) * .5;
    double const radius2 = ( tcp2.second.second - tcp2.second.first ) * .5;

    for( std::size_t s_idx = 0; s_idx < samples_per_segment; s_idx+=2 )
    {
      double const angle = M_PI * s_idx / ( samples_per_segment-2. );
      double const sin_a = std::sin( angle );
      double const cos_a = std::cos( angle );

      s_segment_samples[ seg_idx ][ s_idx   ] = { t1, radius1 * sin_a, radius1 * cos_a + center1 };
      s_segment_samples[ seg_idx ][ s_idx+1 ] = { t2, radius2 * sin_a, radius2 * cos_a + center2 };

      double const h1 = radius1 * cos_a + center1;
      double const h2 = radius2 * cos_a + center2;
      double n_x = ( h1 - h2 ) * -cos_a;
      n_x /= std::hypot( n_x, t2 -t1 );
      double n_y = -sin_a * (1-n_x*n_x);
      double n_z = -cos_a * (1-n_x*n_x);
      
      std::clog << "normal seg " << seg_idx << " norm " << s_idx << " x " << n_x << " y " << n_y << " z " << n_z << std::endl;
      
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

void TubeGlDrawable::drawWallSurface( std::chrono::duration< double > const& t )
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
  for( std::size_t seg_idx = 0; seg_idx < s_segment_vbos.size(); seg_idx++ )
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
    std::cerr << error_message << ": errno " << error_value << " " << std::string( reinterpret_cast< const char*>( gluErrorString( error_value) ) ) << std::endl;
    exit(EXIT_FAILURE);
  }
}

void printShaderInfoLog(GLuint obj)
{
  int infologLength = 0;
  int charsWritten  = 0;
  char *infoLog;
  
  glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
  
  if (infologLength > 0)
  {
    infoLog = (char *)malloc(infologLength);
    glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
    printf("%s\n",infoLog);
    free(infoLog);
  }
}

void printProgramInfoLog(GLuint obj)
{
  int infologLength = 0;
  int charsWritten  = 0;
  char *infoLog;
  
  glGetProgramiv( obj, GL_INFO_LOG_LENGTH, &infologLength );
  if( infologLength > 0 )
  {
    char* infoLog = new char[ infologLength ];
    glGetProgramInfoLog( obj, infologLength, &charsWritten, infoLog );
    std::printf( "%s\n", infoLog );
    delete infoLog;
  }
}

GLuint setupWallSurfaceShaderProgram()
{
  const char* vs_src =  { "#version 130\n"
                          "\n"
                          "uniform mat4 projection_matrix;\n"
                          "uniform mat4 modelview_matrix;\n"
                          "\n"
                          "in vec3 position;\n"
                          "\n"
                          "void main()\n"
                          "{\n"
                          "  gl_Position = projection_matrix * modelview_matrix * vec4( position, 1. );/* ftransform();*/\n" 
                          "}\n"
                        };
  const char* fs_src =  { "#version 130\n"
                          "\n"
                          "out vec4 output_frag_color;\n"
                          "\n"
                          "void main()\n"
                          "{\n"
                          "  output_frag_color = vec4( 1.,1.,1., 1. );\n"
                          "}\n"
                        };

  GLuint vs_handle = glCreateShader( GL_VERTEX_SHADER );
  glShaderSource( vs_handle, 1, &vs_src, nullptr );
  glCompileShader( vs_handle );
  printShaderInfoLog( vs_handle );
  exitOnGLError( "Create vertex shader" );

  GLuint fs_handle = glCreateShader( GL_FRAGMENT_SHADER );
  glShaderSource( fs_handle, 1, &fs_src, nullptr );
  glCompileShader( fs_handle );
  printShaderInfoLog( fs_handle );
  exitOnGLError( "Create fragment shader" );

  GLuint sp_handle = glCreateProgram();
  glAttachShader( sp_handle, vs_handle );
  glAttachShader( sp_handle, fs_handle );

  //glBindAttribLocation( sp_handle, 0, "position");
  //glBindFragDataLocation( sp_handle, 0, "output_frag_color" );

  glLinkProgram( sp_handle );
  printProgramInfoLog( sp_handle );
  exitOnGLError( "Create program" );

  return sp_handle;
}

void TubeGlDrawable::drawWallSurfaceWithShaders( std::chrono::duration< double > const& t )
{
  glEnable( GL_LIGHTING );
  glEnable( GL_LIGHT0 );

  float l0_pos[4] = { t.count(), 0.1, _model->getBox()->position().z(), 1. };
  glLightfv( GL_LIGHT0, GL_POSITION             , l0_pos );
  glLightf ( GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 1. );
  glLightf ( GL_LIGHT0, GL_QUADRATIC_ATTENUATION, .5 );

  glPolygonMode( GL_FRONT, GL_FILL );
  glColor3f( .6,.6,.6 );

  glEnable( GL_COLOR_MATERIAL );
  float mat_ambient[4] = { 0.1, 0.2, 0.2, 1. };
  glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient );
  float mat_diffuse[4] = { 0.7, 0.6, 0.6, 1. };
  glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse );
  float mat_specular[4] = { 0.3, 0.3, 0.1, 1. };
  glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
  float mat_shininess[] = { 10. };
  glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess );

  glShadeModel( GL_SMOOTH );
  glFrontFace( GL_CCW );

  static GLuint sp_handle = setupWallSurfaceShaderProgram();
  glUseProgram( sp_handle );
  {
    //glEnableClientState( GL_NORMAL_ARRAY );

    GLint modelview_loc = glGetUniformLocation( sp_handle, "modelview_matrix" );
    GLfloat modelview_matrix[16]; 
    glGetFloatv( GL_MODELVIEW_MATRIX, modelview_matrix );
    glUniformMatrix4fv( modelview_loc, 1, false, modelview_matrix );
    exitOnGLError( "Setting uniform modelview_matrix");
    
    GLint projection_loc = glGetUniformLocation( sp_handle, "projection_matrix" );
    GLfloat projection_matrix[16];
    glGetFloatv( GL_PROJECTION_MATRIX, projection_matrix );
    glUniformMatrix4fv( projection_loc, 1, false, projection_matrix );
    exitOnGLError( "Setting uniform projection_matrix");

    for( std::size_t seg_idx = 0; seg_idx < s_segment_vbos.size(); seg_idx++ )
    {
      glBindBuffer( GL_ARRAY_BUFFER, s_segment_vbos[seg_idx] );
      GLint position_loc = glGetAttribLocation( sp_handle, "position" );
      glEnableVertexAttribArray( position_loc );// 1rst attribute buffer : vertices
      glVertexAttribPointer(  position_loc,
                              3,         // size
                              GL_DOUBLE, // type
                              GL_FALSE,  // normalized?
                              0,         // stride
                              nullptr    // array buffer offset
                            );
      exitOnGLError( "Setting glEnableVertexAttribPointer");
        glDrawArrays( GL_TRIANGLE_STRIP, 0, s_segment_samples[seg_idx].size() );
      glDisableVertexAttribArray( position_loc );
    }

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    //glDisableClientState( GL_NORMAL_ARRAY );
  }
  glUseProgram( 0 );


  glDisable( GL_LIGHT0 );
  glDisable( GL_LIGHTING );
}
