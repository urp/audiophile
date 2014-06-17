# pragma once

namespace controller 
{
  class InputEventHandler
  {
    public:

      struct keyboard_event
      {
        typedef char key_type;
        key_type key;

        enum modifier_type 
        {   SHIFT_ACTIVE = 1
          ,  CTRL_ACTIVE = 2
          ,   ALT_ACTIVE = 4
        };
        typedef int modifier_mask_type;

        modifier_mask_type modifier_mask;

        double mouse_pos[2];
      };

      // Return true if event was handled.
      virtual bool handle( const keyboard_event& ) = 0;

  }; // InputEventHandler

} // controller::
