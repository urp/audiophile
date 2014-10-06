#version 400

layout(std140) uniform Matrices 
{
  mat4 pvm_matrix;
  mat4  vm_matrix;
  mat4   n_matrix;
} matrices;

layout(std140) uniform Light
{
  vec4  position;
  float constant_attentuation;
  float linear_attentuation;
  float quadratic_attentuation;
} light;

in vec3 position;
in vec3 normal;

out Data 
{
  vec3 normal;
  vec3 eye;
  vec3 light_dir;
} DataOut;

void main()
{
  vec4 pos = matrices.vm_matrix * vec4( position, 1. );

  vec4 lp = matrices.vm_matrix * light.position;

  DataOut.normal   = normalize( mat3( matrices.n_matrix ) * normal );
  DataOut.eye      = vec3( -pos );
  DataOut.light_dir = vec3( lp - pos );

  gl_Position = matrices.pvm_matrix * vec4( position, 1. ); 
}