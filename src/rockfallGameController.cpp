#include "rockfallGameController.hpp"

// =============================
// === GAME CONTROLLER CLASS ===
// =============================

const float RockfallGameController::MAX_BOMB_CHANCE     = 0.75f;
const float RockfallGameController::MIN_BOMB_CHANCE     = 0.1f;
const float RockfallGameController::MIN_ROCK_SIZE       = 0.1f;
const float RockfallGameController::MAX_ROCK_SIZE       = 2.0f;
const int   RockfallGameController::PLAYER_Y_POSITION   = 300;
const float RockfallGameController::PLAYER_ACCELERATION = 25;
const int   RockfallGameController::PLAYER_MAX_SPEED    = 250;
const float RockfallGameController::PLAYER_FRICTION     = 0.9f;
const float RockfallGameController::PLAYER_MIN_POS      = 30;
const float RockfallGameController::PLAYER_MAX_POS      = 170;

const ms RockfallGameController::MAX_TIME_BETWEEN_ROCKS = ms(1000);
const ms RockfallGameController::MIN_TIME_BETWEEN_ROCKS = ms(100);

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

RockfallGameController::RockfallGameController() : pause(false), playing(false), score(0), player_size(1.0f), bomb_chance(0), player_position(10)
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
    for (auto &o : falling_objects)
    {
        delete o;
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
    camera.target = {100, 200};
    last_rock   = std::chrono::steady_clock::now();
    time_to_next_rock = ms(2000);
    FallingObstacle::texture.Load("rockfall_game/bomb_", 2, 10, true);
    FallingObstacle::texture.Initialize();
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
    for (auto &obj : falling_objects)
    {
        obj->Draw();
    }
    // Draw player
    DrawTexturePro(player_texture, {0,0,(float)player_texture.width,(float)player_texture.height}, player_hitbox, {0,0}, 0, WHITE);

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
        if (player_velocity > -0.01 && player_velocity < 0.01) player_velocity = 0;
        
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
            time_to_next_rock = ms(rand() % (MAX_TIME_BETWEEN_ROCKS.count() - MIN_TIME_BETWEEN_ROCKS.count()) + MIN_TIME_BETWEEN_ROCKS.count());

            // Bomb spawning logic
            bomb_chance = GetScore() / 1000.0 + MIN_BOMB_CHANCE;
            if (bomb_chance > MAX_BOMB_CHANCE) bomb_chance = MAX_BOMB_CHANCE;

            float roll = (rand() % 1000) / 1000.0;
            if (roll > bomb_chance)
            {
                FallingRock* r = new FallingRock();
                falling_objects.push_front(r);
            }
            else
            {
                FallingObstacle* o = new FallingObstacle();
                falling_objects.push_front(o);
            } 
        }

        CalculatePlayerHitbox();
        // --------------------------------------
        // --- SCORE CALCULATION THINGIEMAGIK ---
        // --------------------------------------
        for (std::list<FallingObject*>::iterator object = falling_objects.begin(); object != falling_objects.end(); ++object)
        {
            bool rmobstacle = false;
            (*object)->Update();
            if (CheckCollisionCircleRec((*object)->GetPosition(), rock_textures[0].width/2, player_hitbox) && (*object)->GetValue() != value_bomb)
            {
                unsigned char v = (*object)->GetValue() + 1;
                score       += v;
                player_size += v / 100.0f;
                if (player_size > MAX_ROCK_SIZE)
                    player_size = MAX_ROCK_SIZE;
                rmobstacle = true;
            }
            else if ((*object)->GetPosition().y > (PLAYER_Y_POSITION + 50))
            {
                if ((*object)->GetValue() != value_bomb)
                    player_size -= 0.01;

                rmobstacle = true;
                if (player_size < MIN_ROCK_SIZE)
                    player_size = MIN_ROCK_SIZE;
            }

            if (rmobstacle)
            {
                delete (*object);
                object = falling_objects.erase(object);
            }
        }
    }
}

// ============================
// === FALLING OBJECT CLASS ===
// ============================


const int          RockfallGameController::FallingObject::MIN_ROT_SPEED = -50;
const int          RockfallGameController::FallingObject::MAX_ROT_SPEED =  50;
const unsigned int RockfallGameController::FallingObject::MIN_FALL_SPEED = 30;
const unsigned int RockfallGameController::FallingObject::MAX_FALL_SPEED = 50;

void RockfallGameController::FallingObject::RandomizeValues()
{
    rotation       = (rand() % 36000) / 100.0f;
    fall_speed     = rand() % (MAX_FALL_SPEED + MIN_FALL_SPEED) + MIN_FALL_SPEED;
    rotation_speed = rand() % (MAX_ROT_SPEED + abs(MIN_ROT_SPEED)) + abs(MIN_ROT_SPEED);
    position       = {float(rand() % int(PLAYER_MAX_POS) + PLAYER_MIN_POS / 2), 0};
}

RockfallGameController::FallingObject::FallingObject() : fall_speed(MIN_FALL_SPEED), rotation(0), rotation_speed(0), position({0,0}), value(value_default)
{
}

Vector2 RockfallGameController::FallingObject::GetPosition() const
{
    return position;
}

unsigned char RockfallGameController::FallingObject::GetValue() const
{
    return value;
}

// ==============================
// === FALLING OBSTACLE CLASS ===
// ==============================

AnimatedTexture RockfallGameController::FallingObstacle::texture = AnimatedTexture();

RockfallGameController::FallingObstacle::FallingObstacle(bool random_values)
{
    if (random_values)
    {
        RandomizeValues();
    }
    value = value_bomb;
}

void RockfallGameController::FallingObstacle::Draw() const
{
    Transform2D t = {position, rotation, 1};
    texture.Draw(t);
}

void RockfallGameController::FallingObstacle::Update()
{
    rotation   += rotation_speed * GetFrameTime();
    position.y += fall_speed     * GetFrameTime();
    texture.Update();
}

// ==========================
// === FALLING ROCK CLASS ===
// ==========================

const Color RockfallGameController::FallingRock::DEFAULT_TINT = { 255, 255, 255, 255 };
const Color RockfallGameController::FallingRock::RARE_TINT    = { 205, 100, 255, 255 };
const Color RockfallGameController::FallingRock::GOLDEN_TINT  = { 255, 235, 100, 255 };

const float RockfallGameController::FallingRock::RARE_CHANCE   = 0.100f;
const float RockfallGameController::FallingRock::GOLDEN_CHANCE = 0.025f;


RockfallGameController::FallingRock::FallingRock(bool randomValues) : texture_id(0)
{
    if (randomValues)
    {    
        RandomizeValues();
        texture_id     = rand() % rock_textures.size();
        float num = rand() % 10000 / 10000.0;
        value = value_default;
        if (num > GOLDEN_CHANCE && num < RARE_CHANCE)
            value = value_rare;
        else if (num < GOLDEN_CHANCE)
            value = value_golden;
    }
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
    position.y += fall_speed     * GetFrameTime();
    rotation   += rotation_speed * GetFrameTime();
}
