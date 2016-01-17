#version 420 core
#define MAX_WAVE_NUMBER 50
#define MAX_INSTACE 100
#define M_PI 3.1415926535897932384626433832795
#define lambda (-1)
//  Define complex math func
#define cx_mul(a, b) vec2(a.x*b.x-a.y*b.y, a.x*b.y+a.y*b.x)

layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec3 v_normal;

uniform vec3 LightPosition_worldspace;
uniform vec3 EyePosition;
uniform vec3 instance_offset[MAX_INSTACE];

uniform float time;
uniform int N;
uniform int M;
uniform float LengthX;
uniform float LengthZ;

uniform samplerBuffer h0;
uniform samplerBuffer h0Conj;
uniform samplerBuffer dispersion;
uniform samplerBuffer originPos;

out vec3 vColor;

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

void UpdateWave();
vec2 h_(int n, int m);

vec3 v_pos_res;
vec3 normal_res;
void main()
{
    UpdateWave();
    // v_pos_res = v_pos;
    // v_pos_res = texelFetch(originPos, gl_VertexID).rgb;
    // v_pos_res.x = texelFetch(originPos, gl_VertexID).r;

    v_pos_res += instance_offset[gl_InstanceID];
    gl_Position = projection * view * model * vec4(v_pos_res, 1);

    vs_out.position_worldspace = (model * vec4(v_pos_res, 1)).xyz;

    vec3 v_pos_camspace = (view * model * vec4(v_pos_res, 1)).xyz;
    vs_out.eyeDirection_cameraspace = EyePosition - v_pos_camspace;

    vec3 light_pos_camspace = (view * vec4(LightPosition_worldspace, 1)).xyz;
    vs_out.lightDirection_cameraspace = light_pos_camspace + vs_out.eyeDirection_cameraspace;

    vs_out.normal_cameraspace = (view * model * vec4(v_normal,0)).xyz;
}

void UpdateWave()
{
    vec2 sum_comp = vec2(0, 0);
    vec2 sum_displacement = vec2(0, 0);
    vec3 sum_normal = vec3(0, 0, 0);
    for(int m=0; m<M; m++)for(int n=0; n<N; n++)
    {
        vec2 K = vec2(M_PI * (2*n - N) / LengthX
                    , M_PI * (2*m - M) / LengthZ);
        vec2 h_compo = h_(n, m);

        float KdotX = dot(K, v_pos.xz);
        vec2 e_compo = vec2(cos(KdotX), sin(KdotX));
        vec2 hc = cx_mul(h_compo, e_compo);

        sum_comp += hc;

        sum_normal += vec3(- K.x * hc.y, 0, - K.y * hc.y);

        float lengthK = length(K);
        if(lengthK == 0)
          continue;
        sum_displacement += vec2( K.x / lengthK * hc.y, K.y / lengthK * hc.y);
    }

    v_pos_res.y = sum_comp.x;
    vec3 originPos_fetch = texelFetch(originPos, gl_VertexID).rgb;
    v_pos_res.x = v_pos.x + lambda * sum_displacement.x;
    v_pos_res.z = v_pos.z + lambda * sum_displacement.y;
    normal_res = normalize(vec3(0, 1, 0) - sum_normal);

}

vec2 h_(int n, int m)
{
    int pos = m * M + n;
    float dispersionT = texelFetch(dispersion, pos).r * time;
    vec2 e_compo = vec2(cos(dispersionT), sin(dispersionT));
    vec2 e_compoConj = vec2(cos(-dispersionT), sin(-dispersionT));
    vec2 h0_fetch = texelFetch(h0, pos).rg;
    vec2 h0Conj_fetch = texelFetch(h0Conj, pos).rg;

    return (cx_mul(e_compo, h0_fetch)+cx_mul(e_compoConj, h0Conj_fetch));
}
