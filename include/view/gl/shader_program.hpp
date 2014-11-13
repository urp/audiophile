# pragma once

#include <iostream>
#include <map>
#include <typeindex>

#include "view/gl/shader.hpp"

using std::map;
using std::string;
using std::cout;
using std::endl;

class ShaderProgram
{
  private:
    GLuint programId;   // The unique ID / handle for the shader program
    GLuint shaderCount; // How many shaders are attached to the shader program
    
    // Map of attributes and their binding locations
    map<string,int> attributeLocList;
    
    // Map of uniforms and their binding locations
    map<string,int> uniformLocList;
    
    // Map of uniforms and their binding locations
    struct BlockInfo
    {
      std::type_index type;
      GLint block_index;
      GLint buffer;
    };
    map<string, BlockInfo > uniform_blocks;

  public:
    ShaderProgram()
    {
      programId = glCreateProgram();

      // Initially, we have zero shaders attached to the program
      shaderCount = 0;
    }

    ~ShaderProgram()
    {
      glDeleteProgram(programId);
    }
    
    GLuint const& id() const { return programId; }
    
    // Method to attach a shader to the shader program
    void attachShader(Shader shader)
    {
      glAttachShader( programId, shader.getId() );

      // Increment the number of shaders we have associated with the program
      shaderCount++;
    }

    // Method to link the shader program and display the link status
    void link()
    {
      // If we have at least two shaders (like a vertex shader and a fragment shader)...
      if (shaderCount >= 2)
      {
        glLinkProgram(programId);

        // Check the status
        GLint linkStatus;
        glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);
        if (GL_LINK_STATUS == GL_FALSE)
        {
          cout << "Shader program linking FAILED." << endl;
          std::abort();
        }
        else
        {
          cout << "Shader program linking OK." << endl;
        }
      }
      else
      {
        cout << "Can't link shaders - you need at least 2, but attached shader count is only: " << shaderCount << endl;
        std::abort();
      }
    }
    
    
    // Method to enable the shader program
    void use()
    {
      glUseProgram(programId);
    }
    
    
    // Method to disable the shader program
    void disable()
    {
      glUseProgram(0);
    }
    
    
    // Returns the bound location of a named attribute
    GLuint attribute(const string attribute)
    {
      if ( attributeLocList.find(attribute) != attributeLocList.end() )
      {
        return attributeLocList[attribute];
      }
      else
      {
        cout << "Could not find attribute in shader program: " << attribute << endl;
        std::abort();
      }
    }
    
    
    // Method to returns the bound location of a named uniform
    GLuint uniform(const string uniform)
    {
      if ( uniformLocList.find(uniform) != uniformLocList.end() )
      {
        return uniformLocList[uniform];
      }
      else
      {
        cout << "Could not find uniform in shader program: " << uniform << endl;
        std::abort();
      }
    }

    // Method to add an attrbute to the shader and return the bound location
    int addAttribute(const string attributeName)
    {
      attributeLocList[attributeName] = glGetAttribLocation( programId, attributeName.c_str() );
      
      // Check to ensure that the shader contains an attribute with this name
      if (attributeLocList[attributeName] == -1)
      {
        cout << "Could not add attribute: " << attributeName << " - location returned -1!" << endl;
        std::abort();
      }
      else
      {
        cout << "Attribute " << attributeName << " bound to location: " << attributeLocList[attributeName] << endl;
      }
      
      return attributeLocList[attributeName];
    }
    
    
    // Method to add a uniform to the shader and return the bound location
    int addUniform(const string uniformName)
    {
      uniformLocList[uniformName] = glGetUniformLocation( programId, uniformName.c_str() );
      
      // Check to ensure that the shader contains a uniform with this name
      if (uniformLocList[uniformName] == -1)
      {
        cout << "Could not add uniform: " << uniformName << " - location returned -1!" << endl;
        std::abort();
      }
      else
      {
        cout << "Uniform " << uniformName << " bound to location: " << uniformLocList[uniformName] << endl;
      }

      return uniformLocList[uniformName];
    }

    void bindUniformBlock( std::string const& name, GLuint binding_point )
    {
      GLint block_index = glGetUniformBlockIndex( id(), name.c_str() );
      glUniformBlockBinding( id(), block_index, binding_point );
    }
};