#version 330

// Input
in vec4 color_of_fragment;
in vec2 texture_coord;
in vec3 world_position;
in vec3 world_normal;

uniform sampler2D texture_1;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    vec3 color = texture2D(texture_1, texture_coord).xyz;
    vec4 aux = color_of_fragment;
    out_color = vec4(color, 1);
}
