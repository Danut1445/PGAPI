#version 330

// Input
in vec4 color_of_fragment;
in vec2 texture_coord;
in vec3 world_position;
in vec3 world_normal;

uniform sampler2D texture_1;
uniform samplerCube texture_cubemap;
uniform vec3 camera_position;

// Output
layout(location = 0) out vec4 out_color;

vec3 myReflect()
{
    return texture(texture_cubemap, reflect(world_position-camera_position, normalize(world_normal))).xyz;
}


vec3 myRefract(float refractive_index)
{
    return texture(texture_cubemap, refract(world_position-camera_position, world_normal, refractive_index)).xyz;
}

void main()
{
    /*
    vec3 color = texture2D(texture_1, texture_coord).xyz;
    vec4 aux = color_of_fragment;
    out_color = vec4(color, 1);
    */
    vec4 color_fireworks = vec4(texture2D(texture_1, texture_coord).xyz, 1);
    vec4 color_reflection = vec4(myReflect(), 1);
    vec4 color = vec4(0.75f * color_fireworks[0] + 0.25f * color_reflection[0],
                     0.75f * color_fireworks[1] + 0.25f * color_reflection[1],
                     0.75f * color_fireworks[2] + 0.25f * color_reflection[2],
                     1);
    out_color = color;
}
