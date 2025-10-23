#include "gameObject.hxx"

GameObject::GameObject(Texture2D _texture, Transform2D _transform) : transform(_transform), texture(_texture)
{
}

GameObject::GameObject(Transform2D _transform) : transform(_transform)
{
}

GameObject::GameObject(Vector2 position)
{
    transform.position = position;
}

GameObject::GameObject(Vector2 position, float scale)
{
    transform.position = position;
    transform.scale    = scale;
}

GameObject::~GameObject()
{
    UnloadTexture(texture);
}

Transform2D &GameObject::GetTransform()
{
    return transform;
}

const Transform2D &GameObject::GetTransform() const
{
    return transform;
}

const Texture2D &GameObject::GetTexture() const
{
    return texture;
}

void GameObject::Draw() const
{
    Vector2 drawPos = { transform.position.x - (texture.width / 2), transform.position.y - (texture.height / 2)};
    DrawTextureEx(texture, drawPos, transform.rotation, transform.scale, WHITE);
}

void GameObject::Update()
{
}
