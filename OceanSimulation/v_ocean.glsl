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

struct WavePar
{
  vec2 WaveDir;
  int WaveNumber;
  float GlobalSteepness;
  float WaveLength;
  float Speed;
  float KAmpOverLen;
};


layout(std140, binding = 1) uniform WaveParameters
{
  // vec2 WaveDir;
  // int WaveNumber;
  // float GlobalSteepness;
  // float WaveLength;
  // float Speed;
  // float KAmpOverLen;
  WavePar waves[2];
};

out VS_OUT
{
  vec3 color;
  vec3 normal;
  vec2 WaveDir;
}vs_out;

#define M_PI 3.1415926535897932384626433832795
void main()
{


  vec2 WaveD = waves[1].WaveDir;
  int WaveNumber = waves[1].WaveNumber;
  float GlobalSteepness = waves[1].GlobalSteepness;
  float WaveLength = waves[1].WaveLength;
  float Speed = waves[1].Speed;
  float KAmpOverLen = waves[1].KAmpOverLen;


  float Amplitude = WaveLength * KAmpOverLen;
  float Omega = 2*M_PI / WaveLength;
  float Phase = Speed * Omega;
  float Steepness = GlobalSteepness/(Omega * Amplitude * WaveNumber);

  float CosTerm = cos(Omega * dot(WaveD, v_pos.xz) + Phase * time);

  // Compute Position
  vec3 newPos;
  newPos.x = v_pos.x + (Steepness * Amplitude * WaveD.x * CosTerm);
  newPos.z = v_pos.z + (Steepness * Amplitude * WaveD.y * CosTerm);
  newPos.y = Amplitude * sin(Omega * dot(WaveD, v_pos.xz) + Phase * time);

  gl_Position = projection * view * model * vec4(newPos, 1);

  // Compute Normal
  vec3 normal;
  // normal.x = -(WaveX);


  vs_out.WaveDir = WaveD;
  //vs_out.normal = normal;
}
