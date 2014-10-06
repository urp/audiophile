#version 400

layout(std140) uniform Materials
{
    vec4 diffuse;
    vec4 ambient;
    vec4 specular;
    float shininess;
};

layout(std140) uniform Light
{
  vec4  position;
  float constant_attentuation;
  float linear_attentuation;
  float quadratic_attentuation;
} light;

in Data
{
    vec3 normal;
    vec3 eye;
    vec3 light_dir;
} DataIn;

out vec4 colorOut;

void main() 
{
 
    vec3 n = normalize(DataIn.normal);
    vec3 l = normalize(DataIn.light_dir);

    float intensity = max(dot(n,l), 0.0);
    float att = 1.;
    vec4 spec = vec4(0.0);
    if (intensity > 0.0) 
    {
      float dist = length( DataIn.light_dir );
      att = 1. / ( light.constant_attentuation + light.linear_attentuation * dist + light.quadratic_attentuation * dist * dist );

      vec3 e = normalize(DataIn.eye);
      vec3 h = normalize(l + e);
      float intSpec = max(dot(h,n), 0.0);
      spec = specular * pow(intSpec, shininess);
    }

    colorOut = max( att * ( intensity * diffuse + spec ), ambient);
}