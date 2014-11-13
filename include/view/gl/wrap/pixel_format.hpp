// pixel_format.hpp
// Copyright (C) 2006-2011  Peter Urban (peter.urban@s2003.tu-chemnitz.de)
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#pragma once

#include  "gl/headers.hpp"

namespace view
{
  namespace gl
  {
    //---| pixel_format

    template< size_t Components, bool SwapRGB = false >
    class pixel_format
    { public:
      //typedef     GLvoid    pixel_format;
      //static const  GLenum    gl_enum = 0;
    };//specializations can be found below

    template< bool Swap >
    struct pixel_format< 1, Swap >
    {
      static constexpr GLenum gl_enum() { return GL_R; }
    };

    template< >
    struct pixel_format< 2, false >
    {
      static constexpr GLenum gl_enum() { return GL_RG; }
    };

    template< >
    struct pixel_format< 3, false >
    {
      static constexpr GLenum gl_enum() { return GL_RGB; }
    };

    template< >
    struct pixel_format< 3, true >
    {
      static constexpr GLenum gl_enum() { return GL_BGR; }
    };

    template< >
    struct pixel_format< 4, false >
    {
      static constexpr GLenum gl_enum() { return GL_RGBA; }
    };

    template< >
    struct pixel_format< 4, true >
    {
      static constexpr GLenum gl_enum() { return GL_BGRA; }
    };

  }// of gl::

} // of ::view::
