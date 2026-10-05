#version 330

// Input
in vec4 color_of_fragment;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    vec4 aux = color_of_fragment;
    out_color = color_of_fragment;
}
