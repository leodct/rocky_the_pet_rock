#ifndef ANIMATED_TEXTURE_H
#define ANIMATED_TEXTURE_H

#include "globals.hxx"
#include "transform.hxx"
#include <chrono>

class AnimatedTexture{
private:
    std::vector<Texture2D> frames;
    std::chrono::milliseconds ms_per_frame;
    std::chrono::steady_clock::time_point last_frame_change;
    int current_frame;
    bool loop;
    bool play;

    std::string texture_names;
    bool initialized;
    
    void LoadFrames(std::string texture_name, int frame_count);
public:
    AnimatedTexture();
    // IMPORTANT: frame_count considers the texture names start in texture_1.png and end in texture_frame_count.png!!
    AnimatedTexture(std::string texture_name, int frame_count, int fps = 60, bool _loop = true);
    ~AnimatedTexture();

    void Initialize();
    bool IsInitialized() const;

    void Load(std::string texture_name, int frame_count, int fps = 60, bool _loop = true);

    void Play();
    void Pause();
    void Resume();
    void Stop();
    void Reset();

    void Draw(Transform2D transform) const;
    void Update();
};

#endif