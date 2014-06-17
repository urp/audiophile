# pragma once

# include "view/gl_renderer.hpp"

# include "flappy_box/model/tube.hpp"

# include <GL/glew.h>

namespace flappy_box
{
  namespace view
  {
    class TubeGlDrawable : public ::view::GlRenderer::Drawable
    {
      public:
        typedef std::pair< std::chrono::steady_clock::time_point, std::chrono::steady_clock::time_point > control_point_bounds_type;

        TubeGlDrawable( std::shared_ptr< model::Tube > const& );

        virtual void visualize( ::view::GlRenderer&, ::view::GlutWindow& ) override;

      private:

        void updateLines( std::size_t, flappy_box::model::Tube::control_point_map_type::const_iterator const& );
        void drawLines();
        void updateWallPoints( std::size_t, flappy_box::model::Tube::control_point_map_type::const_iterator const& );
        void drawWallPoints();
        void updateWallSurface( std::size_t, flappy_box::model::Tube::control_point_map_type::const_iterator const& );
        void drawWallSurface();

        std::shared_ptr< model::Tube const > _model;

        std::chrono::steady_clock::time_point _first_timestamp;
        control_point_bounds_type _cp_bounds;
        GLuint lineVBOs[2];
        std::vector< std::array< double, 3 > > lower_vertices;
        std::vector< std::array< double, 3 > > upper_vertices;

        std::vector< GLuint > p_segment_vbos;
        std::vector< std::vector< std::array< double, 3 > > > p_segment_samples;

        std::vector< GLuint > s_segment_vbos;
        std::vector< std::vector< std::array< double, 3 > > > s_segment_samples;
        
    }; // TubeGlDrawable
  }
}
