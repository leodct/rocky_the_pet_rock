#ifndef ROCKFALL_H
#define ROCKFALL_H

#include "globals.hxx"
#include "scene.hpp"
#include <chrono>

#define ms std::chrono::milliseconds

class RockfallGameController{
private:
    // Main variables
    bool pause;
    bool playing;
    int  score;
    float rock_size;
    static const float MIN_ROCK_SIZE;
    static const float MAX_ROCK_SIZE;
    
    // Textures related variables
    static Texture2D player_texture;
    static std::vector<Texture2D> rock_textures;

    // Time related variables
    static const ms MIN_TIME_BETWEEN_ROCKS;
    static const ms MAX_TIME_BETWEEN_ROCKS;
    ms time_since_rock;

    // Others
    UIContainer *ui;
    Scene scene;
public:
    RockfallGameController();
    ~RockfallGameController();

    void Pause();
    void UnPause();
    void TogglePause();
    void StartGame();
    void EndGame();

    int   GetScore();
    float GetRockSize();
    float GetMaxRockSize();
    float GetMinRockSize();
    bool  GetPauseStatus() const;
    Texture2D &GetPlayerTexture();
    std::vector<Texture2D> &GetRockTextures();

    void Draw();
    void Update();

    friend class FallingRock;
};

class FallingRock{
private:
    unsigned char texture_id;
    float         rotation;
    unsigned int  fall_speed;
    static const unsigned int MAX_FALL_SPEED,
                              MIN_FALL_SPEED;
    
    void RandomizeValues();
public:
    FallingRock(bool randomValues = true);
};

#endif