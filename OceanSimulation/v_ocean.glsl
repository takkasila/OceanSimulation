#version 420 core
#define MAX_WAVE_NUMBER 50
#define M_PI 3.1415926535897932384626433832795

layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec3 normal;

uniform float time;
uniform int WaveNumber;
uniform float GlobalSteepness;

layout(std140, binding = 0) uniform MVP
{
  mat4 model;
  mat4 view;
  mat4 projection;
};

struct WavePar
{
  vec2 WaveDir;
  float WaveLength;
  float Speed;
  float KAmpOverLen;
};


layout(std140, binding = 1) uniform WaveParameters
{
  WavePar waves[MAX_WAVE_NUMBER];
};

out VS_OUT
{
  vec3 color;
  vec3 normal;
}vs_out;

void main()
{
  vec3 pos_res = vec3(0);
  vec3 normal_res = vec3(0);
  for(int i = 0; i < WaveNumber; i++)
  {
    float Amplitude = waves[i].WaveLength * waves[i].KAmpOverLen;
    float Omega = 2*M_PI / waves[i].WaveLength;
    float Phase = waves[i].Speed * Omega;
    float Steepness = GlobalSteepness/(Omega * Amplitude * WaveNumber);
    float CosTerm = cos(Omega * dot(waves[i].WaveDir, v_pos.xz) + Phase * time);
    float SinTerm = sin(Omega * dot(waves[i].WaveDir, v_pos.xz) + Phase * time);

    // Compute Position
    vec3 newPos;
    newPos.x = v_pos.x + (Steepness * Amplitude * waves[i].WaveDir.x * CosTerm);
    newPos.z = v_pos.z + (Steepness * Amplitude * waves[i].WaveDir.y * CosTerm);
    newPos.y = Amplitude * sin(Omega * dot(waves[i].WaveDir, v_pos.xz) + Phase * time);

    pos_res += newPos;

    // Compute Normal
    vec3 normal;
    normal.x = -(waves[i].WaveDir.x * Omega * Amplitude * CosTerm);
    normal.z = -(waves[i].WaveDir.y * Omega * Amplitude * CosTerm);
    normal.y = 1-(Steepness * Omega * Amplitude * SinTerm);

    normal_res += normal;
  }
  gl_Position = projection * view * model * vec4(pos_res, 1);
  vs_out.normal = normalize(normal_res);
  vs_out.color = vec3(1, 1, 1);

}
