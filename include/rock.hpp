#ifndef ROCK_H
#define ROCK_H

#include "raylib.h"
#include "transform.hxx"
#include "globals.hxx"
#include "gameObject.hxx"

class Rock : public GameObject {
private:
    COSMETICS   equipped_cosmetic;
    void LoadRockTexture();
    void InitRock();
    void DrawRock() const;
public:
    Rock(Transform2D _transform);
    Rock(Vector2 position);
    Rock(Vector2 position, float scale);

    void      SetCosmetic(COSMETICS new_cosmetic);
    COSMETICS GetEquippedCosmetic() const;

    void Draw() const override;
    void Update() override;
};
#endif