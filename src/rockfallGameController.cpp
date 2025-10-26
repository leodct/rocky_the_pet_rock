#include "rockfallGameController.hpp"

// =============================
// === GAME CONTROLLER CLASS ===
// =============================

const float RockfallGameController::MIN_ROCK_SIZE = 0.1f;
const float RockfallGameController::MAX_ROCK_SIZE = 5.0f;

const ms RockfallGameController::MAX_TIME_BETWEEN_ROCKS = ms(1000);
const ms RockfallGameController::MIN_TIME_BETWEEN_ROCKS = ms(100);

Texture2D RockfallGameController::player_texture = Texture2D();
std::vector<Texture2D> RockfallGameController::rock_textures = {};

RockfallGameController::RockfallGameController() : pause(false), playing(false), score(0), rock_size(1.0f)
{
    ui = new UIContainer();
}

RockfallGameController::~RockfallGameController()
{
    for (auto &t : rock_textures)
    {
        UnloadTexture(t);
    }
}

void RockfallGameController::Pause()
{
    pause = true;
}

void RockfallGameController::UnPause()
{
    pause = false;
}

void RockfallGameController::TogglePause()
{
    pause = !pause;
}

void RockfallGameController::StartGame()
{
    playing = true;
}

void RockfallGameController::EndGame()
{
    playing = false;
}

int RockfallGameController::GetScore()
{
    return score;
}

float RockfallGameController::GetRockSize()
{
    return rock_size;
}

float RockfallGameController::GetMaxRockSize()
{
    return MAX_ROCK_SIZE;
}

float RockfallGameController::GetMinRockSize()
{
    return MIN_ROCK_SIZE;
}

bool RockfallGameController::GetPauseStatus() const
{
    return pause;
}

Texture2D &RockfallGameController::GetPlayerTexture()
{
    return player_texture;
}

std::vector<Texture2D> &RockfallGameController::GetRockTextures()
{
    return rock_textures;
}

void RockfallGameController::Draw()
{

}

void RockfallGameController::Update()
{
    // GAME LOGIC
}

// ==========================
// === FALLING ROCK CLASS ===
// ==========================

const unsigned int FallingRock::MIN_FALL_SPEED = 10;
const unsigned int FallingRock::MAX_FALL_SPEED = 20;

void FallingRock::RandomizeValues()
{
    texture_id = rand() % RockfallGameController::rock_textures.size();
    rotation   = (rand() % 36000) / 100.0f;
    fall_speed = rand() % (MAX_FALL_SPEED + MIN_FALL_SPEED) + MIN_FALL_SPEED;
}

FallingRock::FallingRock(bool randomValues) : texture_id(0), rotation(0), fall_speed(MIN_FALL_SPEED)
{
    if (randomValues)
    {    
        RandomizeValues();
    }
}
