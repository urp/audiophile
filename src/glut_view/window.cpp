# include "glut_view/window.hpp"


# include <GL/freeglut.h>
//# include <GL/freeglut_ext.h>
# include <stdexcept>
# include <iostream>

using namespace audiophile::glut_view;

Window::Window( const std::string& name, size_t width, size_t height, const std::shared_ptr< Renderer >& r, const std::shared_ptr< controller::InputEventHandler >& h )
: _name( name )
, _width( width )
, _height( height )
, _renderer( r )
, _input_event_handler( h )
{
  // GLUT Window Initialization:
  glutInitWindowSize( width, height);
  glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

  _glut_win_id = glutCreateWindow( "audiophile" );
  if( _glut_win_id < 1 ) 
    throw std::logic_error( "glut_view::Window::Window: Could not create GLUT window." );

  glutSetWindowData( this );

  // setup callbacks
  glutDisplayFunc( glutDraw );
  glutReshapeFunc( glutReshape );
  glutKeyboardFunc( glutKeyboard );
}

Window::~Window()
{
  ensureCurrent();
  glutSetWindowData( nullptr );
  glutDestroyWindow( _glut_win_id );
}

void Window::invalidate()
{
  ensureCurrent();
  glutPostRedisplay();
}


void Window::ensureCurrent() const
{
  if( _glut_win_id != glutGetWindow() ) 
  {
    std::clog << "glut_view::Window::ensureCurrent: Switching to window " << _name << " with id " << _glut_win_id << "…" << std::endl;
    glutSetWindow( _glut_win_id );
  }
}

unsigned int Window::width() const
{
  return _width;
}

unsigned int Window::height() const
{
  return _height;
}


std::shared_ptr< const Renderer > Window::renderer() const
{ 
  return _renderer; 
}

std::shared_ptr< audiophile::controller::InputEventHandler > Window::input_event_handler()
{
  return _input_event_handler;
}

std::shared_ptr< const audiophile::controller::InputEventHandler > Window::input_event_handler() const
{
  return _input_event_handler;
}


void Window::glutDraw() 
{ 
  Window* win = static_cast< Window* >( glutGetWindowData() );
  if( win )
  {
    win->renderer()->glDraw( *win );
  }
  else throw std::out_of_range( "Could not get pointer to Window." );
}

void Window::glutReshape( int width, int height )
{
  Window* win = static_cast< Window* >( glutGetWindowData() );
  if( win ) 
  {
    win->_width = width;
    win->_height= height;
    win->renderer()->glResize( *win ); 
  }
  else throw std::out_of_range( "Could not get pointer to Window." );
}

void Window::glutKeyboard(unsigned char glut_key, int mouse_x, int mouse_y)
{
  Window* win = static_cast< Window* >( glutGetWindowData() );
  if( not win )
    throw std::out_of_range( "Could not get pointer to Window." );

  controller::InputEventHandler::keyboard_event ev;
  typedef controller::InputEventHandler::keyboard_event event_type;
  switch( glut_key )
  {
    case '0': ev.key = event_type::KEY_0; break;
    case '1': ev.key = event_type::KEY_1; break;
    case '2': ev.key = event_type::KEY_2; break;
    case '3': ev.key = event_type::KEY_3; break;
    case '4': ev.key = event_type::KEY_4; break;
    case '5': ev.key = event_type::KEY_5; break;
    case '6': ev.key = event_type::KEY_6; break;
    case '7': ev.key = event_type::KEY_7; break;
    case '8': ev.key = event_type::KEY_8; break;
    case '9': ev.key = event_type::KEY_9; break;

    case 'q': ev.key = event_type::KEY_Q; break;
    case 'w': ev.key = event_type::KEY_W; break;
    case 'e': ev.key = event_type::KEY_E; break;
    case 'r': ev.key = event_type::KEY_R; break;
    case 't': ev.key = event_type::KEY_T; break;
    case 'z': ev.key = event_type::KEY_Z; break;
    case 'u': ev.key = event_type::KEY_U; break;
    case 'i': ev.key = event_type::KEY_I; break;
    case 'o': ev.key = event_type::KEY_O; break;
    case 'p': ev.key = event_type::KEY_P; break;
    case 'a': ev.key = event_type::KEY_A; break;
    case 's': ev.key = event_type::KEY_S; break;
    case 'd': ev.key = event_type::KEY_D; break;
    case 'f': ev.key = event_type::KEY_F; break;
    case 'g': ev.key = event_type::KEY_G; break;
    case 'h': ev.key = event_type::KEY_H; break;
    case 'j': ev.key = event_type::KEY_J; break;
    case 'k': ev.key = event_type::KEY_K; break;
    case 'l': ev.key = event_type::KEY_L; break;
    case 'y': ev.key = event_type::KEY_Y; break;
    case 'x': ev.key = event_type::KEY_X; break;
    case 'c': ev.key = event_type::KEY_C; break;
    case 'v': ev.key = event_type::KEY_V; break;
    case 'b': ev.key = event_type::KEY_B; break;
    case 'n': ev.key = event_type::KEY_N; break;
    case 'm': ev.key = event_type::KEY_M; break;

    // key above tab case '': ev.key = event_type::KEY_; break;
    //case '°': ev.key = event_type::KEY_DEGREE; break;
    case '!': ev.key = event_type::KEY_EXCLAMATION_MARK; break;
    case '\"': ev.key = event_type::KEY_BACKSLASH; break;
    //case '§': ev.key = event_type::KEY_PARAGRAPH; break;
    case '$': ev.key = event_type::KEY_DOLLAR; break;
    case '%': ev.key = event_type::KEY_PERCENT; break;
    case '&': ev.key = event_type::KEY_AND; break;
    case '/': ev.key = event_type::KEY_SLASH; break;
    case '{': ev.key = event_type::KEY_CURLY_OPEN; break;
    case '(': ev.key = event_type::KEY_PARENTHESIS_OPEN; break;
    case '[': ev.key = event_type::KEY_BRACKET_OPEN; break;
    case ')': ev.key = event_type::KEY_PARENTHESIS_CLOSE; break;
    case ']': ev.key = event_type::KEY_BRACKET_CLOSE; break;
    case '=': ev.key = event_type::KEY_EQUAL; break;
    case '}': ev.key = event_type::KEY_CURLY_CLOSE; break;
    case '?': ev.key = event_type::KEY_QUESTION_MARK; break;
    case '\\': ev.key = event_type::KEY_BACKSLASH; break;
    // TODO case '\'': ev.key = event_type::KEY_; break;
    // TODO case '`': ev.key = event_type::KEY_; break;
    // TODO case '¸': ev.key = event_type::KEY_; break;

    case '*': ev.key = event_type::KEY_TIMES; break;
    case '+': ev.key = event_type::KEY_PLUS; break;
    case '~': ev.key = event_type::KEY_TILDE; break;
    case '#': ev.key = event_type::KEY_HASH; break;
    // TODO case '\'': ev.key = event_type::KEY_; break;

    case ',': ev.key = event_type::KEY_COMMA; break;
    case ';': ev.key = event_type::KEY_SEMICOLON; break;
    case '.': ev.key = event_type::KEY_SENTENCE_MARK; break;
    case ':': ev.key = event_type::KEY_COLON; break;
    case '-': ev.key = event_type::KEY_MINUS; break;
    // TODO case '_': ev.key = event_type::KEY_; break;

    // TODO case '<': ev.key = event_type::KEY_; break;
    // TODO case '>': ev.key = event_type::KEY_; break;
    // TODO case '|': ev.key = event_type::KEY_; break;

    default: throw std::out_of_range( "glut_view::Window::glutKeyboard: unsupported key." );
  }

  //TODO: fill event with more keys & infos

  win->input_event_handler()->handleKeyboardEvent( ev );
}

std::shared_ptr< Renderer > Window::renderer()
{
  return _renderer;
}

void Window::setRenderer(const std::shared_ptr< Renderer >& r)
{
  if( not r ) throw std::logic_error( "glut_view::Window::setRenderer: Invalid renderer." );
  _renderer = r;

  ensureCurrent();
  r->glInitialize( *this );
}