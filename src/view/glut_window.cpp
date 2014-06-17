# include "view/glut_window.hpp"

# include <GL/glew.h>
# include <GL/freeglut.h>

# include <iostream>

using namespace ::view;

GlutWindow::GlutWindow( const std::string& name, size_t width, size_t height, const std::shared_ptr< GlRenderer >& r, const std::shared_ptr< controller::InputEventHandler >& h )
: _name( name )
, _width( width )
, _height( height )
, _renderer( r )
, _input_event_handler( h )
{
  // GLUT GlutWindow Initialization:
  glutInitWindowSize( width, height );
  glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );

  _glut_win_id = glutCreateWindow( name.c_str() );
  if( is_closed() ) 
    throw std::logic_error( "::view::GlutWindow::GlutWindow: Could not create GLUT window." );

  GLenum err = glewInit();
  if (GLEW_OK != err)
  {
    /* Problem: glewInit failed, something is seriously wrong. */
    std::fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
  }

  glutSetWindowData( this );

  // setup callbacks
  glutDisplayFunc( glutDisplay );
  glutReshapeFunc( glutReshape );
  glutKeyboardFunc( glutKeyboard );
  glutCloseFunc( glutClose );
}

GlutWindow::~GlutWindow()
{
  close();
}

void GlutWindow::invalidate()
{
  if( is_closed() )
  {
    std::clog << "view::GlutWindow::ensureCurrent: Window was already closed." << std::endl;
    return;
  }
  ensureCurrent();
  glutPostRedisplay();
}

void GlutWindow::ensureCurrent() const
{
  if( is_closed() )
  {
    std::clog << "view::GlutWindow::ensureCurrent: Window was already closed." << std::endl;
    return;
  }
  if( _glut_win_id != glutGetWindow() ) 
  {
    std::clog << "view::GlutWindow::ensureCurrent: Switching to window " << _name << " with id " << _glut_win_id << "…" << std::endl;
    glutSetWindow( _glut_win_id );
  }
}

unsigned int GlutWindow::width() const
{
  return _width;
}

unsigned int GlutWindow::height() const
{
  return _height;
}

std::shared_ptr< GlRenderer const > GlutWindow::renderer() const
{ 
  return _renderer; 
}

std::shared_ptr< ::controller::InputEventHandler > GlutWindow::input_event_handler()
{
  return _input_event_handler;
}

std::shared_ptr< ::controller::InputEventHandler const > GlutWindow::input_event_handler() const
{
  return _input_event_handler;
}

bool GlutWindow::is_closed() const
{
  return _glut_win_id <= 0;
}

void GlutWindow::close()
{
  if( is_closed() )
  {
    std::clog << "view::GlutWindow::close: Window was already closed." << std::endl;
    return;
  }
  glutDestroyWindow( _glut_win_id );
  _glut_win_id = -1;
}

void GlutWindow::glutDisplay() 
{
  GlutWindow* win = reinterpret_cast< GlutWindow* >( glutGetWindowData() );
  if( not win )
    throw std::out_of_range( "view::GlutWindow::glutDraw: Could not get pointer to GlutWindow." );

  win->renderer()->visualize_model( *win );
}

void GlutWindow::glutReshape( int width, int height )
{
  GlutWindow* win = reinterpret_cast< GlutWindow* >( glutGetWindowData() );
  if( not win ) 
    throw std::out_of_range( "view::GlutWindow::glutReshape: Could not get pointer to GlutWindow." );

  win->_width = width;
  win->_height= height;
  win->renderer()->resize( *win ); 
}

void GlutWindow::glutKeyboard( unsigned char glut_key, int mouse_x, int mouse_y )
{
  GlutWindow* win = reinterpret_cast< GlutWindow* >( glutGetWindowData() );
  if( not win )
    throw std::out_of_range( "view::GlutWindow::glutKeyboard: Could not get pointer to GlutWindow." );

  if( not win->input_event_handler() )
    std::clog << "view::GlutWindow::glutKeyboard: no InputEventHandler attached (which could handle the event)." << std::endl;

  controller::InputEventHandler::keyboard_event ev;
  ev.key = glut_key;
  ev.mouse_pos[0] = mouse_x;
  ev.mouse_pos[1] = mouse_y;
  int glut_modifiers = glutGetModifiers();
  controller::InputEventHandler::keyboard_event::modifier_mask_type modifier_mask = 0;
  if( glut_modifiers & GLUT_ACTIVE_SHIFT ) modifier_mask |= controller::InputEventHandler::keyboard_event::SHIFT_ACTIVE;
  if( glut_modifiers & GLUT_ACTIVE_CTRL  ) modifier_mask |= controller::InputEventHandler::keyboard_event:: CTRL_ACTIVE;
  if( glut_modifiers & GLUT_ACTIVE_ALT   ) modifier_mask |= controller::InputEventHandler::keyboard_event::  ALT_ACTIVE;
  ev.modifier_mask = modifier_mask;
  //TODO: fill event with more keys & infos

  win->input_event_handler()->handle( ev );
}

void GlutWindow::glutClose() 
{ 
  GlutWindow* win = reinterpret_cast< GlutWindow* >( glutGetWindowData() );
  if( not win )
    throw std::out_of_range( "view::GlutWindow::glutClose: Could not get pointer to GlutWindow." );

  if( win->is_closed() )
  {
    std::clog << "view::GlutWindow::glutClose: Window was already closed." << std::endl;
    return;
  }

  win->_glut_win_id = -1;
}

std::shared_ptr< GlRenderer > GlutWindow::renderer()
{
  return _renderer;
}

void GlutWindow::setRenderer( const std::shared_ptr< GlRenderer >& r )
{
  if( not r ) throw std::logic_error( "::view::GlutWindow::setRenderer: Invalid renderer." );
  _renderer = r;
}