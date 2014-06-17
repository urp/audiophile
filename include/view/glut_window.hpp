# pragma once

# include "view/gl_renderer.hpp"
# include "controller/input_event_handler.hpp"

# include <string>

namespace view
{

  class GlutWindow
  {
    public:

      GlutWindow( const std::string& name, size_t width, size_t height, const std::shared_ptr< GlRenderer >&, const std::shared_ptr< controller::InputEventHandler >& );
      ~GlutWindow();

      unsigned int width() const;
      unsigned int height() const;

      std::shared_ptr< GlRenderer > renderer();
      std::shared_ptr< GlRenderer const > renderer() const;
      void setRenderer( std::shared_ptr< GlRenderer > const& );

      std::shared_ptr< controller::InputEventHandler > input_event_handler();
      std::shared_ptr< controller::InputEventHandler const > input_event_handler() const;

      // Request window redisplay.
      void invalidate();

      // Indicates if window was already closed.
      bool is_closed() const;
      // Allows to manually close the window before the destructor is called.
      void close();

    private:
      // make sure this Window is the current GLUT Window.
      void ensureCurrent() const;

      static void glutDisplay(); 
      static void glutReshape( int width, int height );
      static void glutKeyboard( unsigned char glut_key, int mouse_x, int mouse_y );
      static void glutClose();

      std::string _name;
      size_t      _width;
      size_t      _height;
      int         _glut_win_id;

      std::shared_ptr< GlRenderer > _renderer;
      std::shared_ptr< controller::InputEventHandler > _input_event_handler;

  }; // GlutWindow

} // view::

