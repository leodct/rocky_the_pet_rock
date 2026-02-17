#ifndef ROCKFALL_H
#define ROCKFALL_H

#include "globals.hxx"
#include "AnimatedTexture.hxx"
#include "scene.hpp"
#include <chrono>
#include <list>

#define ms std::chrono::milliseconds

class RockfallGameController : public GameObject{
public:
// ----------------------------
// --- FALLING OBJECT CLASS ---
// ----------------------------
    class FallingObject{
    protected:
        unsigned int  fall_speed;
        float         rotation;
        float         rotation_speed;
        static const unsigned int MAX_FALL_SPEED,
                                  MIN_FALL_SPEED;
        static const int          MIN_ROT_SPEED,
                                  MAX_ROT_SPEED;
        Vector2 position;
        #define value_bomb    255
        #define value_default 0
        #define value_rare    1
        #define value_golden  2
        unsigned char value;
    
        void RandomizeValues();
    public:
        FallingObject();
        virtual ~FallingObject() = default;

        Vector2       GetPosition() const;
        unsigned char GetValue() const;

        virtual void Draw() const = 0;
        virtual void Update() = 0;
    };
// ------------------------------
// --- FALLING OBSTACLE CLASS ---
// ------------------------------
    class FallingObstacle : public FallingObject{
    private:
        static AnimatedTexture texture;
    public:
        FallingObstacle(bool random_values = true);

        void Draw() const override;
        void Update() override;

        friend class RockfallGameController;
    };

// --------------------------
// --- FALLING ROCK CLASS ---
// --------------------------
    class FallingRock : public FallingObject{
    private:
        unsigned char texture_id;
        static const Color DEFAULT_TINT, RARE_TINT, GOLDEN_TINT;
        static const float RARE_CHANCE, GOLDEN_CHANCE;
        
    public:
        FallingRock(bool randomValues = true);

        void Draw() const override;
        void Update() override;
    };
private:
    // Main variables
    static bool  pause;
    static bool  game_over;
    static bool  playing;
    static int   score;
    static float player_size;
    static float bomb_chance;
    static const float MAX_BOMB_CHANCE;
    static const float MIN_BOMB_CHANCE;
    static const float MIN_ROCK_SIZE;
    static const float MAX_ROCK_SIZE;
    static const int   PLAYER_Y_POSITION;
    
    // Texture related variables
    static Texture2D player_texture;
    static std::vector<Texture2D> rock_textures;
    static Texture2D background_texture;
    static void   LoadTextures();
    
    // Time related variables
    static const ms MIN_TIME_BETWEEN_ROCKS;
    static const ms MAX_TIME_BETWEEN_ROCKS;
    static std::chrono::steady_clock::duration   time_to_next_rock;
    static std::chrono::steady_clock::time_point last_rock;
    
    // Game related variables
    static float       player_position;
    static float       player_velocity;
    static const float PLAYER_ACCELERATION;
    static const int   PLAYER_MAX_SPEED;
    static const float PLAYER_FRICTION;
    static const float PLAYER_MIN_POS;
    static const float PLAYER_MAX_POS;
    static std::list<FallingObject*> falling_objects;
    static Rectangle player_hitbox;
    static Rectangle player_draw_area;
    static void CalculatePlayerHitbox();
    
    // Others
    static void WipeData();
    public:
    RockfallGameController();
    ~RockfallGameController();
    
    static void Pause();
    static void UnPause();
    static void TogglePause();
    static void StartGame();
    static void EndGame();

    static int   GetScore();
    static int  &GetScoreRef();
    static float GetPlayerSize();
    static float GetMaxRockSize();
    static float GetMinRockSize();
    static bool  GetPauseStatus();
    static bool  IsGameOver();
    static Texture2D &GetPlayerTexture();
    static std::vector<Texture2D> &GetRockTextures();

    void Draw() const override;
    void Update() override;
};

#endif