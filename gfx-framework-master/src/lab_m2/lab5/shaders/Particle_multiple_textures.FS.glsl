#version 430

// Input
layout(location = 0) in vec2 texture_coord;
layout(location = 1) in float geom_lifetime;
layout(location = 2) in float geom_iLifetime;

// Uniform properties
uniform sampler2D texture_1;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    float lifetime_ratio = geom_lifetime / geom_iLifetime;
    int fire_stage = 3 - int (lifetime_ratio / 0.25f);
    
    // TODO(student): Compute texture coordinates so that,
    // depending on the fire stage (0, 1, 2, 3), a different
    // image of a fire particle is shown.
    // Hint: each stage corresponds to a quarter of the image

    vec2 tex_coord = vec2(texture_coord[0] / 4, texture_coord[1] / 4);
    vec3 color;

    if (fire_stage == 0) {
        color = texture(texture_1, tex_coord).xyz;
    }
    if (fire_stage == 1) {
        color = texture(texture_1, tex_coord + vec2(0.25f, 0)).xyz;
    }
    if (fire_stage == 2) {
        color = texture(texture_1, tex_coord + vec2(0.5f, 0)).xyz;
    }
    if (fire_stage == 3) {
        color = texture(texture_1, tex_coord + vec2(0.75f, 0)).xyz;
    }
    out_color = vec4(color, 1);
}
