#version 330 core

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0;   // Texture
uniform vec4 tintCol;      // Tint color (applied to texture)
uniform float outlineSize;    // Outline thickness
uniform vec4 outlineColor;    // Outline color
uniform int hover;            // 1 if hovered, 0 if not
uniform vec2 texSize;         // Texture size

void main()
{
    // Sample the base texture color
    vec4 baseColor = texture(texture0, fragTexCoord);

    // Small threshold for alpha value to account for floating point precision
    float alphaThreshold = 0.01;

    // If baseColor.alpha is below the threshold, treat as transparent
    if (baseColor.a < alphaThreshold)
    {
        // Check if we are hovered and pixel is transparent (for outline)
        if (hover == 1)
        {
            // Outline detection: check neighboring pixels
            float stepX = outlineSize / texSize.x;
            float stepY = outlineSize / texSize.y;

            bool outlineDetected = false;
            for (float x = -1.0; x <= 1.0; x += 1.0)
            {
                for (float y = -1.0; y <= 1.0; y += 1.0)
                {
                    // Skip the center pixel itself (fragTexCoord)
                    if (x == 0.0 && y == 0.0)
                        continue;

                    // Sample neighboring pixel alpha
                    float neighborAlpha = texture(texture0, fragTexCoord + vec2(x * stepX, y * stepY)).a;
                    if (neighborAlpha > alphaThreshold)
                    {
                        outlineDetected = true;
                        break;
                    }
                }
                if (outlineDetected)
                    break;
            }

            // If outline detected, apply the outline color
            if (outlineDetected)
            {
                finalColor = outlineColor;
                return; // Exit early if an outline is applied
            }
        }

        // If no outline is found, discard the pixel
        discard;
    }


    // Apply the tint color modulation to the base color (ONLY to RGB values, NOT alpha)
    baseColor.r += (tintCol.r - 0.5) * tintCol.a;
    baseColor.g += (tintCol.g - 0.5) * tintCol.a;
    baseColor.b += (tintCol.b - 0.5) * tintCol.a;

    // Apply the modulated color with the original alpha value (no discard here)
    finalColor = baseColor;
}
