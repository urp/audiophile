# include "flappy_box/model/tube.hpp"

using namespace flappy_box::model;

using namespace std::chrono;

Tube::Tube( const std::string& n )
: GameObject( true, n )
{

}

std::pair< bool, Tube::control_point_type > Tube::getVerticalBoundsAt( std::chrono::steady_clock::time_point const& t ) const
{
  auto it_lower = std::prev( _control_points.upper_bound( t ) );
  auto it_upper = _control_points.upper_bound( t );

  if( it_lower->first > t or it_upper->first < t or it_upper == it_lower ) throw;

  if( it_lower == _control_points.end() or it_upper == _control_points.end() )
    return { false, { double( it_lower == _control_points.end() ), double( it_upper == _control_points.end() ) } };

  duration<double> t_diff = it_upper->first - it_lower->first;
  double factor = duration_cast< duration<double> >( t - it_lower->first ).count() / t_diff.count();

//   std::clog << "flappy_box::model::Tube::getVerticalBoundsAt: " << "factor " << factor
//                                                                 << " time " << t.time_since_epoch().count()
//                                                                 << " l/u time (" << it_lower->first.time_since_epoch().count() << ", " << it_upper->first.time_since_epoch().count() << ")"
//                                                                 << " diff " << t_diff.count() << "." << std::endl;

  return { true, { it_lower->second.first  * ( 1. - factor ) + it_upper->second.first  * factor
                 , it_lower->second.second * ( 1. - factor ) + it_upper->second.second * factor } };
}

void Tube::manageControlPoints( std::chrono::steady_clock::time_point const& timestamp )
{
  static std::default_random_engine generator;
  static std::uniform_real_distribution< double > distribution( -.2, .2 );

  std::vector< control_point_map_type::value_type > new_cps;
  
  steady_clock::time_point current_timestamp;
  control_point_type       current_cp;


  if( _control_points.rbegin() == _control_points.rend() )
  {
    // insert intial control_point
    current_timestamp = timestamp - duration_cast< steady_clock::duration >( past_coverage_duration() );
    current_cp        = { - .5 * average_diameter, .5 * average_diameter };
    _control_points.emplace( std::make_pair( current_timestamp, current_cp ) );
  }else
  {
    current_timestamp = _control_points.rbegin()->first; 
    current_cp = _control_points.rbegin()->second;
  }

  // remove_obsolete
  
  auto threshold_time = timestamp - past_coverage_duration();
  if( _control_points.begin()->first < threshold_time - removal_tolerance_duration() )
  {
    control_point_map_type::reverse_iterator last_to_remove( std::find_if( _control_points.rbegin(), _control_points.rend()
                                                                        , [threshold_time]( control_point_map_type::value_type const& tcp ) 
                                                                          { return tcp.first < threshold_time; } 
                                                                        ) 
                                                            );
    if( last_to_remove != _control_points.rend() )
      _control_points.erase( _control_points.begin(), --last_to_remove.base() );
  }

  // cover future interval with control points

  if( current_timestamp <= timestamp + future_coverage_duration() )
    while( current_timestamp <= timestamp + future_coverage_duration() + cover_ahead_duration() )
    { 
      current_timestamp += duration_cast< steady_clock::duration >( control_point_interval() );
      double random_shift = distribution( generator );
      current_cp = { current_cp.first + random_shift, /*current_cp.second + random_shift*/ current_cp.first + random_shift + average_diameter + distribution( generator ) };
      new_cps.emplace_back( current_timestamp, current_cp );
      std::clog << "Tube::addControlPointsAfter: control point: " << current_cp.first << " " << current_cp.second << std::endl;
    }

  _control_points.insert( new_cps.begin(), new_cps.end() ); 

}