#version 400

layout(std140) uniform Matrices 
{
  mat4 pvm_matrix;
} matrices;

in vec3 position;

void main()
{
  gl_Position = matrices.pvm_matrix * vec4( position, 1. ); 
}