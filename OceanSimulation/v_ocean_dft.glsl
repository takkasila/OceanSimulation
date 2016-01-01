#version 420 core
#define MAX_WAVE_NUMBER 50
#define MAX_INSTACE 100
#define M_PI 3.1415926535897932384626433832795

layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec3 v_normal;

uniform vec3 LightPosition_worldspace;
uniform vec3 EyePosition;
uniform vec3 instance_offset[MAX_INSTACE];

layout(std140, binding = 0) uniform MVP
{
  mat4 model;
  mat4 view;
  mat4 projection;
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
  vec3 v_pos_res = v_pos + instance_offset[gl_InstanceID];
  gl_Position = projection * view * model * vec4(v_pos_res, 1);

  vs_out.position_worldspace = (model * vec4(v_pos_res, 1)).xyz;

  vec3 v_pos_camspace = (view * model * vec4(v_pos_res, 1)).xyz;
  vs_out.eyeDirection_cameraspace = EyePosition - v_pos_camspace;

  vec3 light_pos_camspace = (view * vec4(LightPosition_worldspace, 1)).xyz;
  vs_out.lightDirection_cameraspace = light_pos_camspace + vs_out.eyeDirection_cameraspace;

  vs_out.normal_cameraspace = (view * model * vec4(v_normal,0)).xyz;
}
