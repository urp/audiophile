# pragma once

# include "model/game_object.hpp"

# include <chrono>
# include <map>

namespace flappy_box
{

  namespace model
  {
    class Box;
    
    class Tube : public ::model::GameObject
    {
      public:

        typedef std::pair< double, double > control_point_type;
        typedef std::map< std::chrono::steady_clock::time_point, control_point_type > control_point_map_type;

        Tube( const std::string& = "Tube" );

        control_point_type getVerticalBoundsAt( std::chrono::steady_clock::time_point const& t ) const;

        control_point_map_type const& getControlPoints() const { return _control_points; }

        void manageControlPoints( std::chrono::steady_clock::time_point const& );

        void setBox( std::shared_ptr< Box > const& b ) { _box = b; }
        std::shared_ptr< Box > getBox() const { return _box; }

      private:
        static constexpr double average_diameter = 5.;
        static constexpr std::pair< double, double > vertical_shift_bounds() { return { -0.5, 0.5 }; }

        static constexpr std::chrono::duration<double>     control_point_interval() { return std::chrono::duration<double>(   1. ); }
        static constexpr std::chrono::duration<double>   future_coverage_duration() { return std::chrono::duration<double>( 10. ); }
        static constexpr std::chrono::duration<double>       cover_ahead_duration() { return std::chrono::duration<double>( 5. ); }
        static constexpr std::chrono::duration<double>     past_coverage_duration() { return std::chrono::duration<double>( 10. ); }
        static constexpr std::chrono::duration<double> removal_tolerance_duration() { return std::chrono::duration<double>( 5. ); }

        control_point_map_type _control_points;
        std::shared_ptr< Box > _box;

    }; // Tube

  } // model::
} // flappy_box::