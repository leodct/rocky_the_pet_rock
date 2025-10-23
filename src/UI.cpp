#include "UI.hpp"
#include "callbacks.hpp"
#include <iostream>

using namespace UI;
// Static member initialization
Color Button::TINT_PRESS = { 150, 150, 150, 255 };

UIElement::UIElement()
{
    draw_order = 0;
}

UIElement::UIElement(Vector2 position)
{
    transform.position = position;
}

UIElement::UIElement(Transform2D _transform)
{
    transform = _transform;
}

UIElement::~UIElement()
{
    UnloadTexture(texture);
}

int UIElement::GetDrawOrder() const
{
    return draw_order;
}

void UIElement::SetDrawOrder(int _order)
{
    draw_order = _order;
    if (draw_order < MIN_DRAW_ORDER) draw_order = MIN_DRAW_ORDER;
    if (draw_order > MAX_DRAW_ORDER) draw_order = MAX_DRAW_ORDER;
}

UIContainer::~UIContainer()
{
    for (auto& element : elements)
    {
        delete element.second;
    }
}

void UIContainer::AddElement(std::string id, UIElement *_element)
{
    elements.emplace(id, _element);
}

void UIContainer::RemoveElement(std::string id)
{
    delete elements[id];
    elements.erase(id);
}

UIElement* UIContainer::GetElement(std::string id)
{
    auto it = elements.find(id);
    if (it != elements.end()) {
        return it->second;
    }
    return nullptr;}

const UIElement* UIContainer::GetElement(std::string id) const
{
    auto it = elements.find(id);
    if (it != elements.end()) {
        return it->second;
    }
    return nullptr;
}

void UIContainer::Update()
{
    for (auto& element : elements){
        element.second->Update();
    }
}

void UIContainer::Draw() const
{
    for (int i = MIN_DRAW_ORDER; i < MAX_DRAW_ORDER; i++)
    {
        for (auto& element : elements){
            if (element.second->GetDrawOrder() == i)
                element.second->Draw();
        }
    }
}

void Button::InitButton()
{
    hover = false;
    press = false;
    callbackFunction = DefaultCallback;
    TextureSetup();
}

void Button::TextureSetup()
{
    hitbox = Rectangle{transform.position.x - (texture.width * transform.scale) / 2, transform.position.y - (texture.height * transform.scale) / 2, (float)texture.width * transform.scale, (float)texture.height * transform.scale};
    Image img = LoadImageFromTexture(texture);
    ImageResizeCanvas(&img, img.width + 8, img.height + 8, 4, 4, {0, 0, 0, 0});
    UnloadTexture(texture);
    texture = LoadTextureFromImage(img);
    UnloadImage(img);
}

// Button methods
Button::Button() : hover(false), press(false) {
    InitButton();
}

Button::Button(Vector2 position)
{
    transform.position = position;
    InitButton();
}

Button::Button(Vector2 position, float rotation)
{
    transform.position = position;
    transform.rotation = rotation;
    InitButton();
}

Button::Button(Transform2D _transform)
{
    transform = _transform;
    InitButton();
}

Button::Button(Texture2D _texture)
{
    texture = _texture;
    InitButton();
}

Button::Button(Texture2D _texture, Vector2 position)
{
    texture            = _texture;
    transform.position = position;
    InitButton();
}

Button::Button(Texture2D _texture, Vector2 position, float rotation)
{
    texture            = _texture;
    transform.position = position;
    transform.rotation = rotation;
    InitButton();
}

Button::Button(Texture2D _texture, Transform2D _transform)
{
    texture   = _texture;
    transform = _transform;
    InitButton();
}

bool Button::IsPressed() const {
    return press;
}

bool Button::IsReleased() const {
    return press && !IsMouseButtonDown(MOUSE_BUTTON_LEFT);
}

bool Button::IsHover() const {
    return hover;
}

void Button::DefineOnPressCallback(std::function<void()> callback)
{
    callbackFunction = callback;
}

void Button::Update() {
    Vector2 mousePos = GetMousePosition();

    hover = CheckCollisionPointRec(mousePos, hitbox);
    
    if (press && !IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
        callbackFunction();
        press = false;
    }
    if (hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        press = true;
    }
    else if (!hover){
        press = false;
    }
}

void Button::Draw() const {
    Vector2 drawPos = {transform.position.x - (texture.width * transform.scale) / 2, transform.position.y - (texture.height * transform.scale) / 2};

    // Set all shader values before entering shader mode
    ConfigOutlineShader(IsHover(), transform.scale * 0.75);

    Vector2 texSize = {(float)texture.width * transform.scale, (float)texture.height * transform.scale};
    int texSizeLoc = GetShaderLocation(outline_shader, "texSize");
    SetShaderValue(outline_shader, texSizeLoc, &texSize, SHADER_UNIFORM_VEC2);

    // Color modulation
    float np = !IsPressed();
    Vector4 colorMod = {
        np, np, np, gameSettings.highlight_strength
    };
    int colorModLoc = GetShaderLocation(outline_shader, "tintCol");
    SetShaderValue(outline_shader, colorModLoc, &colorMod, SHADER_UNIFORM_VEC4);

    // If hovered, use shader mode
    if (hover) {
        BeginShaderMode(outline_shader);
        DrawTextureEx(texture, drawPos, transform.rotation, transform.scale, WHITE);
        EndShaderMode();
    }
    else {
        // Normal state, no shader applied
        DrawTextureEx(texture, drawPos, transform.rotation, transform.scale, WHITE);
    }
}

Panel::Panel(Transform2D _transform, Vector2 _dimensions, Color _col, Color _edge_col, unsigned int _edge_thickness) : col(_col), edge_col(_edge_col), dimensions(_dimensions), edge_thickness(_edge_thickness)
{
    transform = _transform;
}

void Panel::Draw() const
{
    DrawRectangle(transform.position.x, transform.position.y, dimensions.x, dimensions.y, col);
    if (edge_thickness)
    {
        DrawRectangleLinesEx(Rectangle{transform.position.x, transform.position.y, dimensions.x, dimensions.y}, edge_thickness, edge_col);
    }
}

void UI::Panel::Update()
{
}
