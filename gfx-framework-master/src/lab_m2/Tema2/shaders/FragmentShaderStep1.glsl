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
layout(location = 1) out vec4 out_original;

// Local variables
vec2 textureCoord = vec2(texture_coord.x, (flipVertical != 0) ? 1 - texture_coord.y : texture_coord.y); // Flip texture
vec2 mouseRealPosition = vec2(mousePosition.x, (flipVertical != 0) ? screenSize.y - mousePosition.y : mousePosition.y);

float gaussianBlur(int x, float sigma) {
    return exp(-0.5f * ((x * x) / (sigma * sigma))) / (sqrt(6.14f) * sigma);
}

vec4 blur_orizontal(int blurRadius) {
    vec2 texelSize = 1.0f / screenSize;
    vec4 sum = vec4(0);
    float sigma = blurRadius * 0.69f;
    float samples = 0.0f;
    for (int i = -blurRadius; i <= blurRadius; i++) {
        float pondere = gaussianBlur(i, sigma);
        sum += texture(textureImage, textureCoord + vec2(i, 0) * texelSize) * pondere;
        samples += pondere;
    }
    return sum / samples;
}

void main()
{
    vec2 texelSize = 1.0 / vec2(screenSize);
    float dist = sqrt(pow(mouseRealPosition.x * texelSize.x - texture_coord.x, 2) + pow(mouseRealPosition.y * texelSize.y - texture_coord.y, 2));
    float normalizeDist;
    int blurAmount = 0;
    float blurScale;

    if (dist >= radius) {
        normalizeDist = (dist - radius) / (2 * radius);
        blurScale = clamp(normalizeDist, 0.0f, 1.0f);
        blurAmount = int(40 * blurScale);
    }
    
    if (blurAmount > 0) {
        out_color = blur_orizontal(blurAmount);
    } else {
        out_color = texture(textureImage, textureCoord);    
    }
    out_original = vec4(texture(textureImage, textureCoord).xyz, blurAmount);
}
