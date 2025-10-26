#include "rockfallGameController.hpp"

// =============================
// === GAME CONTROLLER CLASS ===
// =============================

const float RockfallGameController::MIN_ROCK_SIZE = 0.1f;
const float RockfallGameController::MAX_ROCK_SIZE = 5.0f;
const int   RockfallGameController::PLAYER_Y_POSITION = 500;
const int   RockfallGameController::player_speed      = 10;

const ms RockfallGameController::MAX_TIME_BETWEEN_ROCKS = ms(1000);
const ms RockfallGameController::MIN_TIME_BETWEEN_ROCKS = ms(100);

Texture2D RockfallGameController::player_texture = Texture2D();
std::vector<Texture2D> RockfallGameController::rock_textures = {};
Texture2D RockfallGameController::background_texture = Texture2D();

void RockfallGameController::CalculatePlayerHitbox()
{
    player_hitbox = Rectangle{player_position - (player_texture.width*player_size / 2), PLAYER_Y_POSITION - (player_texture.height*player_size / 2), player_texture.width * player_size, player_texture.height * player_size};
}

RockfallGameController::RockfallGameController() : pause(false), playing(false), score(0), player_size(1.0f)
{
    ui      = new UIContainer();
    pauseui = new UIContainer();
    CalculatePlayerHitbox();
}

RockfallGameController::~RockfallGameController()
{
    delete ui;
    delete pauseui;
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

float RockfallGameController::GetPlayerSize()
{
    return player_size;
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

void RockfallGameController::Draw() const
{
    // Draw background
    DrawTextureEx(background_texture, {0,0}, 0, 1, WHITE);
    // Draw rocks
    for (auto &rock : rocks)
    {
        rock.Draw();
    }
    // Draw player
    DrawTextureEx(player_texture, {player_position, PLAYER_Y_POSITION}, 0, player_size, WHITE);

    ui->Draw();
    if (pause)
        pauseui->Draw();
}

void RockfallGameController::Update()
{
    if (!pause && playing)
    {
        if (IsKeyDown(KEY_A))
        {
            player_position -= player_speed * GetFrameTime();
        }
        if (IsKeyDown(KEY_D))
        player_position += player_speed * GetFrameTime();

        CalculatePlayerHitbox();

        for (std::vector<FallingRock>::iterator rock = rocks.begin(); rock != rocks.end(); ++rock)
        {
            rock->Update();
            if (CheckCollisionCircleRec(rock->GetPosition(), rock_textures[0].width, player_hitbox))
            {
                unsigned char v = rock->GetValue();
                score       += v;
                player_size += v / 100.0f;
                if (player_size > MAX_ROCK_SIZE)
                    player_size = MAX_ROCK_SIZE;
                rocks.erase(rock);
            }
            else if (rock->GetPosition().y < (PLAYER_Y_POSITION - (player_hitbox.height / 2)))
            {
                if (--score < 0)
                    score = 0;
                player_size -= rock->GetValue();
                if (player_size < MIN_ROCK_SIZE)
                    player_size = MIN_ROCK_SIZE;
            }
        }
    }
}

// ==========================
// === FALLING ROCK CLASS ===
// ==========================

const int RockfallGameController::FallingRock::MIN_ROT_SPEED = -50;
const int RockfallGameController::FallingRock::MAX_ROT_SPEED =  50;

const unsigned int RockfallGameController::FallingRock::MIN_FALL_SPEED = 10;
const unsigned int RockfallGameController::FallingRock::MAX_FALL_SPEED = 20;

const Color RockfallGameController::FallingRock::DEFAULT_TINT = { 255, 255, 255, 255 };
const Color RockfallGameController::FallingRock::RARE_TINT    = { 245, 150, 255, 255 };
const Color RockfallGameController::FallingRock::GOLDEN_TINT  = { 255, 235, 100, 255 };

const float RockfallGameController::FallingRock::RARE_CHANCE   = 0.100f;
const float RockfallGameController::FallingRock::GOLDEN_CHANCE = 0.025f;

void RockfallGameController::FallingRock::RandomizeValues()
{
    texture_id     = rand() % rock_textures.size();
    rotation       = (rand() % 36000) / 100.0f;
    fall_speed     = rand() % (MAX_FALL_SPEED + MIN_FALL_SPEED) + MIN_FALL_SPEED;
    rotation_speed = rand() % (MAX_ROT_SPEED + abs(MIN_ROT_SPEED)) + abs(MIN_ROT_SPEED);
    float num = rand() % 10000 / 10000;
    if (num > RARE_CHANCE + GOLDEN_CHANCE)
        value = value_default;
    else if (num > GOLDEN_CHANCE)
        value = value_golden;
    else
        value = value_rare;
}

RockfallGameController::FallingRock::FallingRock(bool randomValues) : texture_id(0), rotation(0), fall_speed(MIN_FALL_SPEED), value(value_default), position(Vector2{0,0})
{
    if (randomValues)
    {    
        RandomizeValues();
    }
}

Vector2 RockfallGameController::FallingRock::GetPosition() const
{
    return position;
}

unsigned char RockfallGameController::FallingRock::GetValue() const
{
    return value;
}

void RockfallGameController::FallingRock::Draw() const
{
    Color tint = DEFAULT_TINT;
    if (value == value_rare)
        tint = RARE_TINT;
    else if (value == value_golden)
        tint = GOLDEN_TINT;

    DrawTextureEx(rock_textures[texture_id], position, rotation, 1, tint);
}

void RockfallGameController::FallingRock::Update()
{
    position.y += fall_speed * GetFrameTime();
    rotation   += rotation_speed * GetFrameTime();
}
