#version 430

// Input
layout(location = 0) in vec2 texture_coord;

// Uniform properties
uniform sampler2D texture_1;

// Output
layout(location = 0) out vec4 out_color;

void main()
{
    vec3 color = texture2D(texture_1, texture_coord).xyz;
    out_color = vec4(color, 1);

    vec2 aux = texture_coord * 2.0 - vec2(1.0, 1.0);

    float distance = length(aux);

    if (distance < 0.3) {
        out_color = vec4(1.0, 0.0, 0.0, 1.0); 
    }
    else if (distance < 0.5) {
        float angle = atan(aux.y, aux.x);
        float squiggle = 0.05 * sin(10.0 * angle);
        if ((distance - 0.4 - squiggle) < 0.04) {
            out_color = vec4(1.0, 1.0, 1.0, 1.0);
        } else {
            discard;
        }
    }
    
    else {
        discard;
    }
}