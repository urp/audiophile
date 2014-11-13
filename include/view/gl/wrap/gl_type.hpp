
// gl_type.hpp
// Copyright (C) 2006-2013  Peter Urban (urpeter@gmx.ne)
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

# pragma once

# include  "view/gl/headers.hpp"

namespace view
{
  namespace gl
  {
    //---| gl_type

    template<class  T>
    class gl_type
    { public:
      //typedef     GLvoid    type;
      //static const  GLenum    gl_enum = 0;
    };//specializations can be found below

    template<>
    struct gl_type<float>
    {
      typedef GLfloat type;
      static constexpr GLenum const gl_enum = GL_FLOAT;
    };

    template<>
    struct gl_type<double>
    {
      typedef GLdouble type;
      static constexpr GLenum const gl_enum = GL_DOUBLE;
    };

    template<>
    struct gl_type<int>
    {
      typedef GLint type;
      static constexpr GLenum const gl_enum = GL_INT;
    };

    template<>
    class gl_type<unsigned int>
    {
      typedef GLuint type;
      static constexpr GLenum const gl_enum = GL_UNSIGNED_INT;
    };

  }// of gl::
} // of ::view::
