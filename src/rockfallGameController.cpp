#include "rockfallGameController.hpp"

// =============================
// === GAME CONTROLLER CLASS ===
// =============================

const float RockfallGameController::MIN_ROCK_SIZE       = 0.1f;
const float RockfallGameController::MAX_ROCK_SIZE       = 2.0f;
const int   RockfallGameController::PLAYER_Y_POSITION   = 200;
const float RockfallGameController::PLAYER_ACCELERATION = 25;
const int   RockfallGameController::PLAYER_MAX_SPEED    = 250;
const float RockfallGameController::PLAYER_FRICTION     = 0.9f;
const float RockfallGameController::PLAYER_MIN_POS      = 30;
const float RockfallGameController::PLAYER_MAX_POS      = 170;

const ms RockfallGameController::MAX_TIME_BETWEEN_ROCKS = ms(100);
const ms RockfallGameController::MIN_TIME_BETWEEN_ROCKS = ms(1);

Texture2D RockfallGameController::player_texture = Texture2D();
std::vector<Texture2D> RockfallGameController::rock_textures = {};
Texture2D RockfallGameController::background_texture = Texture2D();

#include <iostream>

void RockfallGameController::LoadTextures()
{
    player_texture = LoadTexture((TEXTURES_PATH/"rocky.png").u8string().c_str());
    std::string str = (TEXTURES_PATH/"rockfall_game/rock_").u8string();
    std::string png = ".png";
    rock_textures.resize(9);
    for (int i = 0; i < 9; i++)
    {
        rock_textures[i] = LoadTexture((str + std::to_string(i+1) + png).c_str());
    }
}

void RockfallGameController::CalculatePlayerHitbox()
{
    player_hitbox = Rectangle{player_position + 2, PLAYER_Y_POSITION + 4, player_texture.width * player_size - 4, player_texture.height * player_size - 4};
    player_hitbox.x -= (player_texture.width  / 2) * player_size;
    player_hitbox.y -= (player_texture.height / 2) * player_size;
}

RockfallGameController::RockfallGameController() : pause(false), playing(false), score(0), player_size(1.0f), player_position(10)
{
    ui      = new UIContainer();
    pauseui = new UIContainer();
    CalculatePlayerHitbox();
    LoadTextures();
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
    camera.zoom   = 2.0f;
    camera.target = {100, 100};
    last_rock   = std::chrono::steady_clock::now();
    time_to_next_rock = ms(2000);
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
    DrawRectangleLinesEx(player_hitbox, 2, {200, 20, 250, 255});
    DrawTexturePro(player_texture, {0,0,(float)player_texture.width,(float)player_texture.height}, player_hitbox, {0,0}, 0, WHITE);
    DrawCircleV({player_position, PLAYER_Y_POSITION}, 2, RED);

    ui->Draw();
    if (pause)
        pauseui->Draw();
}

void RockfallGameController::Update()
{
    if (!pause && playing)
    {
        // ------------------------------------
        // --- PLAYER MOVEMENT THINGIEMAGIK ---
        // ------------------------------------
        if (IsKeyDown(KEY_A))
        {
            player_velocity -= PLAYER_ACCELERATION;
            player_velocity = (player_velocity < -PLAYER_MAX_SPEED) ? -PLAYER_MAX_SPEED : player_velocity;
        }
        if (IsKeyDown(KEY_D))
        {
            player_velocity += PLAYER_ACCELERATION;
            player_velocity = (player_velocity > PLAYER_MAX_SPEED) ? PLAYER_MAX_SPEED : player_velocity;
        }
        player_velocity *= PLAYER_FRICTION;
        //if (player_velocity > -0.01 && player_velocity < 0.01) player_velocity = 0;
        
        float npp = player_position + player_velocity * GetFrameTime();
        if (npp-player_size < PLAYER_MIN_POS) npp = PLAYER_MIN_POS+player_size;
        if (npp+player_size > PLAYER_MAX_POS) npp = PLAYER_MAX_POS-player_size;
        player_position = npp;
        
        
        // ----------------------------------
        // --- ROCK SPAWNING THINGIEMAGIK ---
        // ----------------------------------
        if ((std::chrono::steady_clock::now() - last_rock).count() > time_to_next_rock.count())
        {
            last_rock = std::chrono::steady_clock::now();
            time_to_next_rock = ms((rand() % (MIN_TIME_BETWEEN_ROCKS + MAX_TIME_BETWEEN_ROCKS).count()) + MIN_TIME_BETWEEN_ROCKS.count());
            rocks.push_front(FallingRock(true));
        }
        //std::cout << "------------------------" << std::endl
        //          << "Score: " << score << std::endl
        //          << "Rock size: " << player_size << std::endl;
        
        CalculatePlayerHitbox();
        // --------------------------------------
        // --- SCORE CALCULATION THINGIEMAGIK ---
        // --------------------------------------
        for (std::list<FallingRock>::iterator rock = rocks.begin(); rock != rocks.end(); ++rock)
        {
            bool rmrock = false;
            rock->Update();
            if (CheckCollisionCircleRec(rock->GetPosition(), rock_textures[0].width/2, player_hitbox))
            {
                unsigned char v = rock->GetValue() + 1;
                score       += v;
                player_size += v / 100.0f;
                if (player_size > MAX_ROCK_SIZE)
                    player_size = MAX_ROCK_SIZE;
                rmrock = true;
            }
            else if (rock->GetPosition().y > (PLAYER_Y_POSITION + 50))
            {
                player_size -= 0.01;
                rmrock = true;
                //player_size -= rock->GetValue();
                if (player_size < MIN_ROCK_SIZE)
                    player_size = MIN_ROCK_SIZE;
            }

            if (rmrock)
                rock = rocks.erase(rock);
        }
    }
}



// ==========================
// === FALLING ROCK CLASS ===
// ==========================

const int RockfallGameController::FallingRock::MIN_ROT_SPEED = -50;
const int RockfallGameController::FallingRock::MAX_ROT_SPEED =  50;

const unsigned int RockfallGameController::FallingRock::MIN_FALL_SPEED = 30;
const unsigned int RockfallGameController::FallingRock::MAX_FALL_SPEED = 50;

const Color RockfallGameController::FallingRock::DEFAULT_TINT = { 255, 255, 255, 255 };
const Color RockfallGameController::FallingRock::RARE_TINT    = { 205, 100, 255, 255 };
const Color RockfallGameController::FallingRock::GOLDEN_TINT  = { 255, 235, 100, 255 };

const float RockfallGameController::FallingRock::RARE_CHANCE   = 0.100f;
const float RockfallGameController::FallingRock::GOLDEN_CHANCE = 0.025f;

void RockfallGameController::FallingRock::RandomizeValues()
{
    texture_id     = rand() % rock_textures.size();
    rotation       = (rand() % 36000) / 100.0f;
    fall_speed     = rand() % (MAX_FALL_SPEED + MIN_FALL_SPEED) + MIN_FALL_SPEED;
    rotation_speed = rand() % (MAX_ROT_SPEED + abs(MIN_ROT_SPEED)) + abs(MIN_ROT_SPEED);
    position       = {float(rand() % int(PLAYER_MAX_POS) + PLAYER_MIN_POS / 2), 0};
    float num = rand() % 10000 / 10000.0;
        value = value_default;
    if (num > GOLDEN_CHANCE && num < RARE_CHANCE)
        value = value_rare;
    else if (num < GOLDEN_CHANCE)
        value = value_golden;
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

    
    float texsize = rock_textures[0].width;
    Vector2 origin = {texsize / 2, texsize / 2};
    Rectangle textureRec = {0,0,texsize,texsize};
    Rectangle destRec    = {position.x, position.y, texsize, texsize};
    DrawTexturePro(rock_textures[texture_id], textureRec, destRec, origin, rotation, tint);
}

void RockfallGameController::FallingRock::Update()
{
    position.y += fall_speed * GetFrameTime();
    rotation   += rotation_speed * GetFrameTime();
}
