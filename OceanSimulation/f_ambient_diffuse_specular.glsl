#version 420 core

uniform vec3 LightPosition_worldspace;
uniform vec3 DirectionalLight_direction_worldspace;

layout(std140, binding = 0) uniform MVP
{
  mat4 model;
  mat4 view;
  mat4 projection;
};

in VS_OUT
{
  vec3 position_worldspace;
  vec3 normal_cameraspace;
  vec3 eyeDirection_cameraspace;
  vec3 lightDirection_cameraspace;
} fs_in;

out vec3 color;

void main()
{

  vec3 MaterialDiffuseColor = vec3(2/255.0, 132/255.0, 130/255.0);
  vec3 MaterialAmbientColor = vec3(0.1, 0.1, 0.1) * MaterialDiffuseColor;
  vec3 MaterialSpecularColor = vec3(0.1, 0.0, 0.0);

  // Point Light
  vec3 LightColor = vec3(1, 1, 1);
  float LightPower = 7000;

  float distance_to_light = length(LightPosition_worldspace - fs_in.position_worldspace);

  vec3 n = normalize(fs_in.normal_cameraspace);

  vec3 l = normalize(fs_in.lightDirection_cameraspace);

  float cosTheta = clamp(dot(n,l), 0, 1);

  vec3 E = normalize(fs_in.eyeDirection_cameraspace);
  vec3 R = reflect(-l, n);

  float cosAlpha = clamp(dot(E, R), 0, 1);

  color = MaterialAmbientColor
    // + MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance_to_light * distance_to_light)
    + MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha, 32) / (distance_to_light * distance_to_light);

  // Directional Light
  vec3 DirectionalLight_direction_camspace = ( view * model *vec4(DirectionalLight_direction_worldspace, 0)).xyz;
  vec3 Di_Light_direction = normalize(-DirectionalLight_direction_camspace);
  vec3 Di_Light_color = vec3(1, 1, 1);
  float Di_Light_power = 1.8;

  float cosTheta_Di = clamp(dot(n, Di_Light_direction), 0, 1);

  color += MaterialDiffuseColor * Di_Light_color * Di_Light_power * cosTheta_Di;
}
