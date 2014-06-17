# pragma once

# include "model/game_object.hpp"
# include "math.hpp"

# include <chrono>
# include <map>

namespace flappy_box
{

  namespace model
  {
    class Tube : public ::model::GameObject
    {
      public:

        typedef std::pair< double, double > control_point_type;
        typedef std::map< std::chrono::steady_clock::time_point, control_point_type > control_point_map_type;

        Tube( const std::string& = "Tube" );

        std::pair< bool, control_point_type > getVerticalBoundsAt( std::chrono::steady_clock::time_point const& t ) const;

        control_point_map_type const& getControlPoints() const { return _control_points; }

        void manageControlPoints( std::chrono::steady_clock::time_point const& );

      private:
        static constexpr double average_diameter = 5.;
        static constexpr std::chrono::duration<double>     control_point_interval() { return std::chrono::duration<double>(   .2 ); }
        static constexpr std::chrono::duration<double>   future_coverage_duration() { return std::chrono::duration<double>( 10. ); }
        static constexpr std::chrono::duration<double>       cover_ahead_duration() { return std::chrono::duration<double>( 5. ); }
        static constexpr std::chrono::duration<double>     past_coverage_duration() { return std::chrono::duration<double>( 10. ); }
        static constexpr std::chrono::duration<double> removal_tolerance_duration() { return std::chrono::duration<double>( 5. ); }

        control_point_map_type _control_points;

    }; // Box

  } // model::
} // flappy_box::