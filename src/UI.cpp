#include "UI.hpp"
#include "callbacks.hpp"
#include <iostream>

using namespace UI;
// Static member initialization
Color Button::TINT_PRESS = { 150, 150, 150, 255 };

void UIElement::Init()
{
    draw_order = 0;
    enabled = active;
}

UIElement::UIElement(bool _active) : active(_active)
{
    Init();
}

UIElement::UIElement(Vector2 position, bool _active) : active(_active)
{
    Init();
    transform.position = position;
}

UIElement::UIElement(Transform2D _transform, bool _active) : active(_active)
{
    Init();
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

void UIElement::ToggleDisplayState()
{
    active = !active;
}

void UIElement::SetDisplayState(bool _active)
{
    active = _active;
}

bool UIElement::GetDisplayState()
{
    return active;
}

void UIElement::Enable()
{
    enabled = true;
}

void UIElement::Disable()
{
    enabled = false;
}

void UIElement::ToggleEnabled()
{
    enabled = !enabled;
}

bool UIElement::IsEnabled() const
{
    return enabled;
}

UIContainer::UIContainer() : draw_order(0)
{
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

int UIContainer::GetDrawOrder() const
{
    return draw_order;
}

void UIContainer::SetDrawOrder(int _order)
{
    draw_order = _order;
}

void UIContainer::Update()
{
    for (auto& element : elements){
        if (element.second->GetDisplayState())
        element.second->Update();
    }
}

void UIContainer::Draw() const
{
    for (int i = MIN_DRAW_ORDER; i < MAX_DRAW_ORDER; i++)
    {
        for (auto& element : elements){
            if (element.second->GetDrawOrder() == i && element.second->GetDisplayState())
                element.second->Draw();
        }
    }
}

void UIContainer::EnableAll()
{
    for (auto &element : elements)
    {
        element.second->SetDisplayState(true);
    }
}

void UIContainer::DisableAll()
{
    for (auto &element : elements)
    {
        element.second->SetDisplayState(false);
    }
}

void UIContainer::ToggleAll()
{
    for (auto &element : elements)
    {
        element.second->ToggleDisplayState();
    }
}

void UIContainer::SetAllVisibilityTo(bool value)
{
    for (auto &element : elements)
    {
        element.second->SetDisplayState(value);
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
    if (enabled)
    {
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

UI::Label::Label(Transform2D _transform, std::string _text, unsigned int _text_size, Color _text_col, ALIGNMENT _alignment) : text_col(_text_col), text(_text), text_size(_text_size), alignment(_alignment)
{
    transform = _transform;
}

std::string &UI::Label::GetText()
{
    return text;
}

const std::string &UI::Label::GetText() const
{
    return text;
}

void UI::Label::SetText(std::string _text)
{
    text = _text;
}

unsigned int UI::Label::GetFontSize() const
{
    return text_size;
}

void UI::Label::SetFontSize(unsigned int _size)
{
    text_size = _size;
}

UI::Label::ALIGNMENT UI::Label::GetAlignment() const
{
    return alignment;
}

void UI::Label::SetAlignment(ALIGNMENT _alignment)
{
    alignment = _alignment;
}

Color UI::Label::GetTextColor() const
{
    return text_col;
}

void UI::Label::SetTextColor(Color color)
{
    text_col = color;
}

void UI::Label::Update()
{
}

void UI::Label::Draw() const
{
    float spacing = 1;
    Vector2 origin = MeasureTextEx(GetFontDefault(), text.c_str(), text_size, spacing);
    origin.y = origin.y / 2;
    switch (alignment)
    {
        case ALIGNMENT::LEFT:
            origin.x = 0;
            break;
        case ALIGNMENT::RIGHT:
            origin.x = origin.x / 2;
            break;
        default: // case ALIGNMENT::MIDDLE:
            break;
    }
    DrawTextPro(GetFontDefault(), text.c_str(), transform.position, origin, transform.rotation, text_size, spacing, text_col);
}
