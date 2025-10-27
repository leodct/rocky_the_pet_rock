#ifndef ROCKFALL_H
#define ROCKFALL_H

#include "globals.hxx"
#include "scene.hpp"
#include <chrono>
#include <list>

#define ms std::chrono::milliseconds

class RockfallGameController : public GameObject{
public:
    // --------------------------
    // --- FALLING ROCK CLASS ---
    // --------------------------
    class FallingRock{
    private:
        unsigned char texture_id;
        float         rotation;
        float         rotation_speed;
        static const int MIN_ROT_SPEED,
                         MAX_ROT_SPEED;
        unsigned int  fall_speed;
        static const unsigned int MAX_FALL_SPEED,
                                  MIN_FALL_SPEED;
        #define value_default 0
        #define value_rare    1
        #define value_golden  2
        unsigned char value;
        static const Color DEFAULT_TINT, RARE_TINT, GOLDEN_TINT;
        static const float RARE_CHANCE, GOLDEN_CHANCE;
        Vector2 position;
        
        void RandomizeValues();
    public:
        FallingRock(bool randomValues = true);

        Vector2       GetPosition() const;
        unsigned char GetValue() const;
        void Draw() const;
        void Update();
    };
private:
    // Main variables
    bool  pause;
    bool  playing;
    int   score;
    float player_size;
    static const float MIN_ROCK_SIZE;
    static const float MAX_ROCK_SIZE;
    static const int   PLAYER_Y_POSITION;
    
    // Texture related variables
    static Texture2D player_texture;
    static std::vector<Texture2D> rock_textures;
    static Texture2D background_texture;
    void   LoadTextures();
    
    // Time related variables
    static const ms MIN_TIME_BETWEEN_ROCKS;
    static const ms MAX_TIME_BETWEEN_ROCKS;
    std::chrono::steady_clock::duration   time_to_next_rock;
    std::chrono::steady_clock::time_point last_rock;
    
    // Game related variables
    float              player_position;
    float              player_velocity;
    static const float PLAYER_ACCELERATION;
    static const int   PLAYER_MAX_SPEED;
    static const float PLAYER_FRICTION;
    static const float PLAYER_MIN_POS;
    static const float PLAYER_MAX_POS;
    std::list<FallingRock> rocks;
    Rectangle player_hitbox;
    void CalculatePlayerHitbox();
    
    // Others
    UIContainer *ui;
    UIContainer *pauseui;
    public:
    RockfallGameController();
    ~RockfallGameController();
    
    void Pause();
    void UnPause();
    void TogglePause();
    void StartGame();
    void EndGame();

    int   GetScore();
    float GetPlayerSize();
    float GetMaxRockSize();
    float GetMinRockSize();
    bool  GetPauseStatus() const;
    Texture2D &GetPlayerTexture();
    std::vector<Texture2D> &GetRockTextures();

    void Draw() const override;
    void Update() override;
};

#endif