
# pragma once

namespace audiophile
{
  namespace controller 
  {
    /// Application class
    class InputEventHandler
    {
      public:

        struct keyboard_event
        {
          enum key_type
            {
              KEY_0 = '0'
            , KEY_1 = '1'
            , KEY_2 = '2'
            , KEY_3 = '3'
            , KEY_4 = '4'
            , KEY_5 = '5'
            , KEY_6 = '6'
            , KEY_7 = '7'
            , KEY_8 = '8'
            , KEY_9 = '9'

            , KEY_Q = 'q'
            , KEY_W = 'w'
            , KEY_E = 'e'
            , KEY_R = 'r'
            , KEY_T = 't'
            , KEY_Z = 'z'
            , KEY_U = 'u'
            , KEY_I = 'i'
            , KEY_O = 'o'
            , KEY_P = 'p'
            , KEY_A = 'a'
            , KEY_S = 's'
            , KEY_D = 'd'
            , KEY_F = 'f'
            , KEY_G = 'g'
            , KEY_H = 'h'
            , KEY_J = 'j'
            , KEY_K = 'k'
            , KEY_L = 'l'
            , KEY_Y = 'y'
            , KEY_X = 'x'
            , KEY_C = 'c'
            , KEY_V = 'v'
            , KEY_B = 'b'
            , KEY_N = 'n'
            , KEY_M = 'm'

            , KEY_DEGREE
            , KEY_EXCLAMATION_MARK
            , KEY_DASY
            , KEY_PARAGRAPH
            , KEY_DOLLAR
            , KEY_PERCENT
            , KEY_AND
            , KEY_SLASH
            , KEY_CURLY_OPEN
            , KEY_PARENTHESIS_OPEN
            , KEY_BRACKET_OPEN
            , KEY_PARENTHESIS_CLOSE
            , KEY_BRACKET_CLOSE
            , KEY_EQUAL
            , KEY_CURLY_CLOSE
            , KEY_QUESTION_MARK
            , KEY_BACKSLASH

            , KEY_TIMES
            , KEY_PLUS
            , KEY_TILDE
            , KEY_HASH

            , KEY_COMMA
            , KEY_SEMICOLON
            , KEY_SENTENCE_MARK
            , KEY_COLON
            , KEY_MINUS
            };

          enum modifier_type 
            {
              
            };

          key_type key;
          modifier_type modifier;
        };

        virtual bool handleKeyboardEvent( const keyboard_event& ) = 0;
    };

  } // controller::
} // audiophile::