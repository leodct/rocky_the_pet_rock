#include "rock.hpp"
#include "globals.hxx"

#include <iostream>

void Rock::DrawRock() const
{
    Vector2 drawPos = { transform.position.x - (texture.width / 2), transform.position.y - (texture.height / 2)};
    Vector2 rp = {transform.position.x - (texture.width * transform.scale) / 2, transform.position.y - (texture.height * transform.scale) / 2};
    Vector2 mpos = GetScreenToWorld2D(GetMousePosition(), camera);
    Rectangle rr = {rp.x, rp.y, (float)texture.width * transform.scale, (float)texture.height * transform.scale};

    bool shader = highlight && CheckCollisionPointRec(mpos, rr);
    if (shader)
    {
        ConfigOutlineShader(true, camera.zoom / 2);
        BeginShaderMode(outline_shader);
    }
    DrawTextureEx(texture, drawPos, transform.rotation, transform.scale, WHITE);
    if (shader){
        EndShaderMode();
        ConfigOutlineShader(false);
    }
    switch(equipped_cosmetic){
        case COSMETICS::DAISY:      DrawTextureEx(cosmetics_textures[static_cast<int>(COSMETICS::DAISY)], drawPos, transform.rotation, transform.scale, WHITE); break;
        case COSMETICS::SUNGLASSES: DrawTextureEx(cosmetics_textures[static_cast<int>(COSMETICS::SUNGLASSES)], drawPos, transform.rotation, transform.scale, WHITE); break;
        case COSMETICS::RED_BOWTIE: DrawTextureEx(cosmetics_textures[static_cast<int>(COSMETICS::RED_BOWTIE)], drawPos, transform.rotation, transform.scale, WHITE); break;
        default: break; // Case COSMETICS::NONE
    }
}

void Rock::LoadRockTexture()
{
    Image smallTex = LoadImage((TEXTURES_PATH/"rocky.png").string().c_str());
    ImageResizeCanvas(&smallTex, smallTex.width + 4, smallTex.height + 4, 0, 0, {255, 255, 255, 0});
    texture = LoadTextureFromImage(smallTex);
    UnloadImage(smallTex);

    if (texture.width == 0 || texture.height == 0 || smallTex.width == 0 || smallTex.height == 0) {
        std::cerr << "Error: Texture failed to load from image!" << std::endl;
    }
}

void Rock::InitRock()
{
    LoadRockTexture();
    equipped_cosmetic = COSMETICS::NONE;
    highlight = true;
}

Rock::Rock(Transform2D _transform)
{
    transform = _transform;
    InitRock();
}

Rock::Rock(Vector2 _position)
{
    transform.position = _position;
    InitRock();
}

Rock::Rock(Vector2 position, float scale)
{
    transform.position = position;
    transform.scale    = scale;
    InitRock();
}

void Rock::SetCosmetic(COSMETICS new_cosmetic)
{
    equipped_cosmetic = new_cosmetic;
}

COSMETICS Rock::GetEquippedCosmetic() const
{
    return equipped_cosmetic;
}

bool Rock::ShouldHighlight() const
{
    return highlight;
}

void Rock::EnableHighlight()
{
    highlight = true;
}

void Rock::DisableHighlight()
{
    highlight = false;
}

void Rock::ToggleHighlight()
{
    highlight = !highlight;
}

void Rock::SetHighlight(bool _highlight)
{
    highlight = _highlight;
}

void Rock::Draw() const
{
    DrawRock();
}

void Rock::Update()
{
}
