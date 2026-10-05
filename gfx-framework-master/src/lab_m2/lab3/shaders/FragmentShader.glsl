#version 330

// Input
in vec2 f_texture_coord;

// Uniform properties
uniform sampler2D texture_1;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Apply the texture
    vec4 color = texture2D(texture_1, f_texture_coord);

    // TODO(student): Discard when alpha component < 0.75
    if (color.a < 0.75f) {
        discard;
    }

    out_color = color;
}
