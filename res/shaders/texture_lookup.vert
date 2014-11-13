#version 430 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 TexCoord;

out vec3 vTexCoord;

void main()
{
  vTexCoord = TexCoord.xyz;

  // you don't need to project, you're already in NDCs!
  gl_Position = vec4( Position, 1. );
}
