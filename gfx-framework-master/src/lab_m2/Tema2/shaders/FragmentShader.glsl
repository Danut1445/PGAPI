#version 330

// Input
in vec2 texture_coord;

// Uniform properties
uniform sampler2D textureImage;
uniform ivec2 screenSize;
uniform ivec2 mousePosition;
uniform int flipVertical;
uniform int outputMode = 2; // 0: original, 1: grayscale, 2: blur
uniform float radius;

// Output
layout(location = 0) out vec4 out_color;

// Local variables
vec2 textureCoord = vec2(texture_coord.x, (flipVertical != 0) ? 1 - texture_coord.y : texture_coord.y); // Flip texture
vec2 mouseRealPosition = vec2(mousePosition.x, (flipVertical != 0) ? screenSize.y - mousePosition.y : mousePosition.y);


vec4 grayscale()
{
    vec4 color = texture(textureImage, textureCoord);
    float gray = 0.21 * color.r + 0.71 * color.g + 0.07 * color.b; 
    return vec4(gray, gray, gray,  0);
}

vec4 blur_orizontal(int blurRadius) {
    vec2 texelSize = 1.0f / screenSize;
    vec4 sum = vec4(0);
    for (int i = -blurRadius; i <= blurRadius; i++) {
        sum += texture(textureImage, textureCoord + vec2(i, 0) * texelSize);
    }
    float samples = 2 * blurRadius + 1;
    return sum / samples;
}

vec4 blur_vertical(int blurRadius) {
    vec2 texelSize = 1.0f / screenSize;
    vec4 sum = vec4(0);
    for (int i = -blurRadius; i <= blurRadius; i++) {
        sum += texture(textureImage, textureCoord + vec2(0, i) * texelSize);
    }
    float samples = 2 * blurRadius + 1;
    return sum / samples;
}

vec4 blur(int blurRadius)
{
    vec2 texelSize = 1.0f / screenSize;
    vec4 sum = vec4(0);
    for(int i = -blurRadius; i <= blurRadius; i++)
    {
        for(int j = -blurRadius; j <= blurRadius; j++)
        {
            sum += texture(textureImage, textureCoord + vec2(i, j) * texelSize);
        }
    }
        
    float samples = pow((2 * blurRadius + 1), 2);
    return sum / samples;
}

vec4 sobel(){
    vec2 texelSize = 1.0 / vec2(screenSize);
    float gx = 0.0;
    float gy = 0.0;

    float kernelX[9] = float[9](-1, 0, 1, -2, 0, 2, -1, 0, 1);
    float kernelY[9] = float[9](-1, -2, -1, 0, 0, 0, 1, 2, 1);

    int idx = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            vec2 offset = vec2(i, j) * texelSize;
            float sample_image = texture(textureImage, textureCoord + offset).r;
            gx += sample_image * kernelX[idx];
            gy += sample_image * kernelY[idx];
            idx++;
        }
    }

    float edge = length(vec2(gx, gy));


    float threshold = 0.05;
    float edgeValue = edge > threshold ? 1.0 : 0.0;

    return vec4(edgeValue, edgeValue, edgeValue, 1.0);
}

vec4 median(int radius)
{
    int size = 2 * radius + 1;
    int windowPixels = size * size;
    float r[49], g[49], b[49];

    int idx = 0;
    vec2 texelSize = 1.0 / vec2(screenSize);
    for (int i = -radius; i <= radius; i++)
    {
        for (int j = -radius; j <= radius; j++)
        {
            vec4 color = texture(textureImage, textureCoord + vec2(i, j) * texelSize);
            r[idx] = color.r;
            g[idx] = color.g;
            b[idx] = color.b;
            idx++;
        }
    }

    float tmp;
    for (int i = 0; i < windowPixels - 1; i++)
    {
        for (int j = i + 1; j < windowPixels; j++)
        {
            if (r[i] > r[j]) {
                tmp = r[i];
                r[i] = r[j]; 
                r[j] = tmp; 
            }
            if (g[i] > g[j]) {
                tmp = g[i]; 
                g[i] = g[j]; 
                g[j] = tmp;
            }
            if (b[i] > b[j]) {
                tmp = b[i];
                b[i] = b[j];
                b[j] = tmp;
            }
        }
    }

    float medianR = r[windowPixels / 2];
    float medianG = g[windowPixels / 2];
    float medianB = b[windowPixels / 2];

    return vec4(medianR, medianG, medianB, 1.0);
}

void main()
{    
    switch (outputMode)
    {
        case 1:
        {
            out_color = grayscale();
            break;
        }

        case 2:
        {
            out_color = blur(3);
            break;
        }

        case 3:
        {
            out_color = blur_orizontal(3);
            break;
        }

        case 4:
        {
            out_color = blur_vertical(3);
            break;
        }

        default:
            out_color = texture(textureImage, textureCoord);
            break;
    }
    
    
}
