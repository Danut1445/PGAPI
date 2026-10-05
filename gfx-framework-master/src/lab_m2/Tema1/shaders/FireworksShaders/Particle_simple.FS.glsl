#version 430

// Input
layout(location = 0) in vec2 texture_coord;

// Uniform properties
uniform sampler2D texture_1;
uniform vec3 firework_color;

// Output
layout(location = 0) out vec4 out_color;

void main()
{
    vec3 color = texture2D(texture_1, texture_coord).xyz;
    out_color = vec4(firework_color, 1);
}