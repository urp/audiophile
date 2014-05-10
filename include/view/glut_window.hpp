# pragma once

# include "view/gl_renderer.hpp"
# include "controller/input_event_handler.hpp"

# include <memory>
# include <string>

namespace audiophile
{
  namespace view
  {
    // forward
    class GLRenderer;

    class GLUTWindow
    {
      public:

        GLUTWindow( const std::string& name, size_t width, size_t height, const std::shared_ptr< GLRenderer >&, const std::shared_ptr< controller::InputEventHandler >& );
        ~GLUTWindow();

        unsigned int width() const;
        unsigned int height() const;

        std::shared_ptr< GLRenderer > renderer();
        std::shared_ptr< const GLRenderer > renderer() const;
        void setRenderer( const std::shared_ptr< GLRenderer >& );

        std::shared_ptr< controller::InputEventHandler > input_event_handler();
        std::shared_ptr< const controller::InputEventHandler > input_event_handler() const;

        void invalidate();

      private:
        void ensureCurrent() const;

        static void glutDraw(); 
        static void glutReshape( int width, int height );
        static void glutKeyboard( unsigned char glut_key, int mouse_x, int mouse_y );

        std::string _name;
        size_t      _width;
        size_t      _height;
        int         _glut_win_id;

        std::shared_ptr< GLRenderer > _renderer;

        std::shared_ptr< controller::InputEventHandler > _input_event_handler;
    }; // GLUTWindow

  } // view::
} // audiophile::