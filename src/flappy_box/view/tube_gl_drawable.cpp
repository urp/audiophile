# include "flappy_box/view/tube_gl_drawable.hpp"

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
        updateWallPoints( reused_cps, it_old_end );
        updateLines( reused_cps, it_old_end );
      }

      glTranslated( duration_cast< duration<double> >( _first_timestamp - timestamp ).count(), 0., 0. );

      // Draw primitives
      drawWallSurface();
      drawWallPoints();
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
    glBufferData( GL_ARRAY_BUFFER, p_segment_samples[seg_idx].size() * 3 * sizeof(double) , p_segment_samples[seg_idx].data(), GL_STATIC_DRAW);

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
  
  glDeleteBuffers( s_segment_vbos.size() - reused_cps, s_segment_vbos.data() );
  std::copy( s_segment_vbos.end() - reused_cps, s_segment_vbos.end(), s_segment_vbos.begin() );
  s_segment_vbos.resize( _model->getControlPoints().size() - 1 );
  glGenBuffers( s_segment_vbos.size() - reused_cps, &s_segment_vbos[ reused_cps ] );
  
  std::copy( s_segment_samples.end() - reused_cps, s_segment_samples.end(), s_segment_samples.begin() );
  s_segment_samples.resize( s_segment_vbos.size() );
  
  std::size_t seg_idx = reused_cps;
  for( auto cp_it = it_old_end; cp_it != std::prev( cps.end() ) ; ++cp_it )
  {
    auto const& tcp1 = *cp_it;
    auto const& tcp2 = *std::next( cp_it );
    constexpr std::size_t num_s_segment_samples = 20;
    s_segment_samples[ seg_idx ].resize( num_s_segment_samples );

    double t1 = duration_cast< duration<double> >( tcp1.first - _first_timestamp ).count();
    double t2 = duration_cast< duration<double> >( tcp2.first - _first_timestamp ).count();
    double center1 = ( tcp1.second.second + tcp1.second.first ) * .5;
    double center2 = ( tcp2.second.second + tcp2.second.first ) * .5;
    double radius1 = ( tcp1.second.second - tcp1.second.first ) * .5;
    double radius2 = ( tcp2.second.second - tcp2.second.first ) * .5;

    for( std::size_t s_idx = 0; s_idx < num_s_segment_samples; s_idx+=2 )
    {
      double angle = M_PI * s_idx / ( num_s_segment_samples-1. );

      s_segment_samples[ seg_idx ][ s_idx   ] = { t1, radius1 * sin( angle ), radius1 * cos( angle ) + center1 };
      s_segment_samples[ seg_idx ][ s_idx+1 ] = { t2, radius2 * sin( angle ), radius2 * cos( angle ) + center2 };

    }
    
    glBindBuffer( GL_ARRAY_BUFFER, s_segment_vbos[seg_idx] );
    glBufferData( GL_ARRAY_BUFFER, s_segment_samples[seg_idx].size() * 3 * sizeof(double) , s_segment_samples[seg_idx].data(), GL_STATIC_DRAW);
    
    seg_idx++;
  }
}

void TubeGlDrawable::drawWallSurface()
{
  glPointSize( 2. );
  glShadeModel( GL_SMOOTH );
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  glColor3f( .6,.6,.6 );
  glCullFace( GL_FRONT_AND_BACK );
  //glFrontFace( GL_CCW );
  for( std::size_t seg_idx = 0; seg_idx < s_segment_vbos.size(); seg_idx++ )
  {
    glBindBuffer( GL_ARRAY_BUFFER, s_segment_vbos[seg_idx] );
    glVertexPointer( 3, GL_DOUBLE, 0, nullptr );
    glDrawArrays( GL_TRIANGLE_STRIP, 0, s_segment_samples[seg_idx].size() );
  }
}
