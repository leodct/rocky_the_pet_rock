#include "AnimatedTexture.hxx"

void AnimatedTexture::LoadFrames(std::string texture_name, int frame_count)
{
    for (int i = 0; i < frame_count; i++)
    {
        frames[i] = LoadTexture(((TEXTURES_PATH/texture_name).u8string() + std::to_string(i+1) + ".png").c_str());
    }
}

AnimatedTexture::AnimatedTexture()
{
    frames.resize(1);
    frames[0]         = Texture2D{};
    ms_per_frame      = std::chrono::milliseconds(0);
    last_frame_change = std::chrono::steady_clock::now();
    current_frame     = 0;
    loop              = false;
}

AnimatedTexture::AnimatedTexture(std::string texture_name, int frame_count, int fps, bool _loop) : loop(_loop), texture_names(texture_name), initialized(false)
{
    ms_per_frame      = std::chrono::milliseconds(1000 / fps);
    last_frame_change = std::chrono::steady_clock::now();
    frames.resize(frame_count);
    current_frame = 0;
    play = loop;
}

AnimatedTexture::~AnimatedTexture()
{
    for (auto &frame : frames)
    {
        UnloadTexture(frame);
    }
}

void AnimatedTexture::Initialize()
{
    LoadFrames(texture_names, frames.size());
    initialized = true;
}

bool AnimatedTexture::IsInitialized() const
{
    return initialized;
}

void AnimatedTexture::Load(std::string texture_name, int frame_count, int fps, bool _loop)
{
    texture_names = texture_name;
    loop = _loop;
    ms_per_frame      = std::chrono::milliseconds(1000 / fps);
    last_frame_change = std::chrono::steady_clock::now();
    frames.resize(frame_count);
    current_frame = 0;
    play = loop;
    LoadFrames(texture_names, frames.size());
}

void AnimatedTexture::Play()
{
    play = true;
    last_frame_change = std::chrono::steady_clock::now();
}

void AnimatedTexture::Pause()
{
    play = false;
}

void AnimatedTexture::Resume()
{
    Play();
}

void AnimatedTexture::Stop()
{
    Pause();
    Reset();
}

void AnimatedTexture::Reset()
{
    current_frame = 0;
}

void AnimatedTexture::Draw(Transform2D transform) const
{
    if (initialized)
    {
        Rectangle rsource  = {0,0,(float)frames[current_frame].width,(float)frames[current_frame].height};
        Rectangle rdest    = {transform.position.x, transform.position.y, rsource.width * transform.scale, rsource.height * transform.scale};
        Vector2   origin   = {rsource.width / 2, rsource.height / 2};
        DrawTexturePro(frames[current_frame], rsource, rdest, origin, transform.rotation, WHITE);
    }
}

void AnimatedTexture::Update()
{
    if ((std::chrono::steady_clock::now() - last_frame_change) > ms_per_frame && initialized && play)
    {
        current_frame = (current_frame + 1) % frames.size();
        last_frame_change = std::chrono::steady_clock::now();
    }
}
