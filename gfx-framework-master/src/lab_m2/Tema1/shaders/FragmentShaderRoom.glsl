#version 330

// Input
in vec4 color_of_fragment;
in vec2 texture_coord;
in vec3 world_position;
in vec3 world_normal;
flat in int nrtexture;

uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform sampler2D texture_3;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    vec3 color;
    if (nrtexture == 0) {
        color = texture2D(texture_1, texture_coord).xyz;
    }
    if (nrtexture == 1) {
        color = texture2D(texture_2, texture_coord).xyz;
    }
    if (nrtexture == 2) {
        color = texture2D(texture_3, texture_coord).xyz;
    }
    vec4 aux = color_of_fragment;
    out_color = vec4(color, 1);
}
