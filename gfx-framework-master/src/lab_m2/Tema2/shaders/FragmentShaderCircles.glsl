#version 330

// Input
in vec2 texture_coord;

// Uniform properties
uniform sampler2D textureImage;
uniform ivec2 screenSize;
uniform ivec2 mousePosition;
uniform int flipVertical;
uniform float radius;

// Output
layout(location = 0) out vec4 out_color;

// Local variables
vec2 textureCoord = vec2(texture_coord.x, (flipVertical != 0) ? 1 - texture_coord.y : texture_coord.y); // Flip texture
vec2 mouseRealPosition = vec2(mousePosition.x, (flipVertical != 0) ? screenSize.y - mousePosition.y : mousePosition.y);

void main()
{
    vec2 texelSize = 1.0 / vec2(screenSize);
    float dist = sqrt(pow(mouseRealPosition.x * texelSize.x - texture_coord.x, 2) + pow(mouseRealPosition.y * texelSize.y - texture_coord.y, 2));
    
    out_color = texture(textureImage, textureCoord);

    if (dist <= radius + 0.002f && dist >= radius - 0.002f) {
        out_color = vec4(1, 1, 1, 1);
    }

    if (dist <= radius / 2.0f + 0.002f && dist >= radius / 2.0f - 0.002f) {
        out_color = vec4(1, 1, 1, 1);
    }

    if (dist <= radius / 4.0f + 0.002f && dist >= radius / 4.0f - 0.002f) {
        out_color = vec4(1, 1, 1, 1);
    }
}
