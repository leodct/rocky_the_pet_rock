#include "globals.hxx"

Camera2D  camera                          = {};
Texture2D background_texture              = {};
std::vector<Texture2D> cosmetics_textures = {};
Settings gameSettings                     = { false, 90, 0.15f };
bool closeGame                            = false;
bool mouse_over_button                    = false;
bool highlight_rock                       = true;
int  balance                              = 0;

void DrawBackground(bool tiled, Camera2D camera)
{
    Vector2 TL = {camera.target.x - camera.offset.x, camera.target.y - camera.offset.y};
    if (!tiled){
        DrawTextureV(background_texture, TL, WHITE);
    }
    else{ // Tiled
        for (int i = TL.x; i < WINDOW_SIZE.x; i += background_texture.width){
            for (int j = TL.y; j < WINDOW_SIZE.y; j += background_texture.height){
                DrawTextureV(background_texture, Vector2{(float)i,(float)j}, WHITE);
            }
        }
    }
}

void LoadCosmetics()
{
    cosmetics_textures.resize(static_cast<int>(COSMETICS::SIZE));
    cosmetics_textures[0] = Texture2D();
    cosmetics_textures[1] = LoadTexture((COSMETICS_PATH/"daisy.png").u8string().c_str());
    cosmetics_textures[2] = LoadTexture((COSMETICS_PATH/"sunglasses.png").u8string().c_str());
    cosmetics_textures[3] = LoadTexture((COSMETICS_PATH/"red_bowtie.png").u8string().c_str());
}

// --- SHADER SHENANIGANS ---

Shader outline_shader = {};
void InitShaders()
{
    outline_shader = LoadShader(TextFormat("%s/outline.vs", SHADERS_PATH.string().c_str()), TextFormat("%s/outline.fs", SHADERS_PATH.string().c_str()));

    Vector2 texSize = { 64.0f, 64.0f }; // Default (will be updated per texture)
    int texSizeLoc = GetShaderLocation(outline_shader, "texSize");
    SetShaderValue(outline_shader, texSizeLoc, &texSize, SHADER_UNIFORM_VEC2);

    float outlineSize = 2.0f;
    int outlineSizeLoc = GetShaderLocation(outline_shader, "outlineSize");
    SetShaderValue(outline_shader, outlineSizeLoc, &outlineSize, SHADER_UNIFORM_FLOAT);

    Vector4 outlineColor = {1.0f, 1.0f, 1.0f, 1.0f}; // white
    int outlineColorLoc = GetShaderLocation(outline_shader, "outlineColor");
    SetShaderValue(outline_shader, outlineColorLoc, &outlineColor, SHADER_UNIFORM_VEC4);

    Vector4 colorMod = {1, 1, 1, 0}; // Set shader tint alpha to 0% (aka no tint)

    int colorModLoc = GetShaderLocation(outline_shader, "tintCol");
    SetShaderValue(outline_shader, colorModLoc, &colorMod, SHADER_UNIFORM_VEC4);
}

void ConfigOutlineShader(bool outline, float thickness)
{
    int hover = outline;
    int hoverLoc = GetShaderLocation(outline_shader, "hover");
    SetShaderValue(outline_shader, hoverLoc, &hover, SHADER_UNIFORM_INT);

    int outlineSizeLoc = GetShaderLocation(outline_shader, "outlineSize");
    SetShaderValue(outline_shader, outlineSizeLoc, &thickness, SHADER_UNIFORM_FLOAT);
}
