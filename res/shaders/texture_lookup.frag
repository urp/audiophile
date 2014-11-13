#version 430 core

layout (location = 3) uniform sampler2D tex;

in  vec3 vTexCoord;
out vec4 FragColor;

void main()
{
  FragColor = texture( tex, vTexCoord.xy );// vec4(vTexCoord, 1.0);
}