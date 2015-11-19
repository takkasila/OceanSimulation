#version 420 core

layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec3 normal;

uniform float time;

layout(std140, binding = 0) uniform MVP
{
  mat4 model;
  mat4 view;
  mat4 projection;
};

layout(std140, binding = 1) uniform WaveParameters
{
  int WaveNumber;
  float GlobalSteepness;
  float WaveLength;
  float Speed;
  float KAmpOverLen;
  // vec2 WaveDir;
  float WaveX;
  float WaveY;
};

out VS_OUT
{
  vec3 color;
  vec3 normal;
}vs_out;

#define M_PI 3.1415926535897932384626433832795
void main()
{

  float Amplitude = WaveLength * KAmpOverLen;
  float Omega = 2*M_PI / WaveLength;
  float Phase = Speed * Omega;
  float Steepness = GlobalSteepness/(Omega * Amplitude * WaveNumber);

  float CosTerm = cos(Omega * dot(vec2(WaveX, WaveY), v_pos.xz) + Phase * time);

  vec3 newPos;

  newPos.x = v_pos.x + (Steepness * Amplitude * WaveX * CosTerm);
  newPos.z = v_pos.z + (Steepness * Amplitude * WaveY * CosTerm);
  newPos.y = Amplitude * sin(Omega * dot(vec2(WaveX, WaveY), v_pos.xz) + Phase * time);

  gl_Position = projection * view * model * vec4(newPos, 1);
  vs_out.normal = normal;
}
