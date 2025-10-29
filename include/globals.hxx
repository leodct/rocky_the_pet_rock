#include <filesystem>
#include "raylib.h"
#include <vector>
#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>

// Paths
const std::filesystem::path EXECUTABLE_PATH = std::filesystem::current_path(),
                            RESOURCES_PATH  = EXECUTABLE_PATH / "resources",
                            TEXTURES_PATH   = RESOURCES_PATH / "textures",
                            COSMETICS_PATH  = TEXTURES_PATH / "cosmetics",
                            SHADERS_PATH    = RESOURCES_PATH / "shaders";

// Display
const Vector2   WINDOW_SIZE = {360, 640};
extern Camera2D camera;

// Game
extern bool closeGame;
extern bool mouse_over_button;

// Settings
struct Settings{
    bool  vSync;
    int   targetFPS;
    float highlight_strength; // Values must be in the [0, 1] range. Anything above 1 will default to 1, and anything below 0 I don't know but if I were you I wouldn't dare mess with it... Things might happen...
};
extern Settings gameSettings;

// Backgrounds
extern Texture2D background_texture;
void DrawBackground(bool tiled, Camera2D camera);

// Drawing related things
const int MIN_DRAW_ORDER = -100,
          MAX_DRAW_ORDER =  100;

// Cosmetics
enum class COSMETICS {
    NONE,
    DAISY,
    SUNGLASSES,
    RED_BOWTIE,
    SIZE
};
extern std::vector<Texture2D> cosmetics_textures;
void LoadCosmetics();

// --- SHADER STUFF ---
const float OUTLINE_SHADER_THICKNESS_DEFAULT = 2.0f;

extern Shader outline_shader;
void InitShaders();

void ConfigOutlineShader(bool outline, float thickness = OUTLINE_SHADER_THICKNESS_DEFAULT);

#endif