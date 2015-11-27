#version 420 core
#define MAX_WAVE_NUMBER 50
#define M_PI 3.1415926535897932384626433832795

layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec3 normal;

uniform float time;
uniform int WaveNumber;
uniform float GlobalSteepness;
uniform vec3 LightPosition_worldspace;
uniform vec3 EyePosition;

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
  vec3 position_worldspace;
  vec3 normal_cameraspace;
  vec3 eyeDirection_cameraspace;
  vec3 lightDirection_cameraspace;
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
  normal_res = normalize(normal_res);

  gl_Position = projection * view * model * vec4(pos_res, 1);

  vs_out.position_worldspace = (model * vec4(pos_res, 1)).xyz;

  vec3 v_pos_camspace = (view * model * vec4(pos_res, 1)).xyz;
  vs_out.eyeDirection_cameraspace = EyePosition - v_pos_camspace;

  vec3 light_pos_camspace = (view * vec4(LightPosition_worldspace, 1)).xyz;
  vs_out.lightDirection_cameraspace = light_pos_camspace + vs_out.eyeDirection_cameraspace;

  vs_out.normal_cameraspace = (view * model * vec4(normal_res,0)).xyz;
}
