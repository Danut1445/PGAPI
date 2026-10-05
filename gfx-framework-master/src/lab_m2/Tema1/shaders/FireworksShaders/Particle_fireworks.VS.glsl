#version 430

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform vec3 generator_position;
uniform float deltaTime;

out float vert_lifetime;
out float vert_iLifetime;

struct ParticleTema1
{
    vec4 position;
    vec4 speed;
    vec4 iposition;
    vec4 ispeed;
    float delay;
    float iDelay;
    float lifetime;
    float iLifetime;
};


layout(std430, binding = 0) buffer particles {
    ParticleTema1 data[];
};


float rand(vec2 co)
{
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}


void main()
{
    vec3 pos = data[gl_VertexID].position.xyz;
    vec3 spd = data[gl_VertexID].speed.xyz;
    int current_phase;
    float lifetime = data[gl_VertexID].lifetime;

    float dt = deltaTime * 5;

    if (lifetime < 0.7f) {
        pos = pos + spd * dt;
        spd = spd + vec3(0, -1, 0) * dt;

        data[gl_VertexID].position.xyz =  pos;
        data[gl_VertexID].speed.xyz =  spd;
        data[gl_VertexID].lifetime = lifetime + deltaTime;
    } else {
        spd = vec3(data[gl_VertexID].iLifetime * 5, data[gl_VertexID].iDelay * 5, data[gl_VertexID].delay * 5);
        data[gl_VertexID].speed.xyz = spd;
        data[gl_VertexID].lifetime = lifetime - 10;
    }

    gl_Position = Model * vec4(pos + generator_position, 1);
}
