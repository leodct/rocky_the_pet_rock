#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "transform.hxx"

class GameObject{
protected:
    Transform2D transform;
    Texture2D   texture;
public:
    GameObject(Texture2D _texture = {}, Transform2D _transform = {});
    GameObject(Transform2D _transform);
    GameObject(Vector2 position);
    GameObject(Vector2 position, float scale);
    virtual ~GameObject();

    Transform2D&       GetTransform();
    const Transform2D& GetTransform() const;
    const Texture2D&   GetTexture()   const;

    virtual void Draw() const;
    virtual void Update();
};
#endif