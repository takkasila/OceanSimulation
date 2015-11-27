#version 420 core

uniform vec3 LightPosition_worldspace;

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
  vec3 LightColor = vec3(1, 1, 1);
  float LightPower = 50;

  vec3 MaterialDiffuseColor = vec3(1, 0.5, 0.5);
  vec3 MaterialAmbientColor = vec3(0.1, 0.1, 0.1) * MaterialDiffuseColor;
  vec3 MaterialSpecularColor = vec3(0.3, 0.3, 0.3);

  float distance_to_light = length(LightPosition_worldspace - fs_in.position_worldspace);

  vec3 n = normalize(fs_in.normal_cameraspace);

  vec3 l = normalize(fs_in.lightDirection_cameraspace);

  float cosTheta = clamp(dot(n,l), 0, 1);

  vec3 E = normalize(fs_in.eyeDirection_cameraspace);
  vec3 R = reflect(-l, n);

  float cosAlpha = clamp(dot(E, R), 0, 1);

  color = MaterialAmbientColor
    + MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance_to_light * distance_to_light)
    + MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha, 5) / (distance_to_light * distance_to_light);
}
