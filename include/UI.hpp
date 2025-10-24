#ifndef UI_H
#define UI_H

#include <vector>
#include "globals.hxx"
#include <map>
#include "transform.hxx"
#include <functional>
#include "gameObject.hxx"

class UIElement : public GameObject{
protected:
    int  draw_order;
    bool active;
    void Init();
public:
    UIElement();
    UIElement(Vector2 position);
    UIElement(Transform2D _transform);
    virtual ~UIElement();

    int  GetDrawOrder() const;
    void SetDrawOrder(int _order);

    void ToggleDisplayState();
    void SetDisplayState(bool _active);
    bool GetDisplayState();

    virtual void Draw() const = 0;
    virtual void Update() = 0;
};

class UIContainer {
private:
    std::map<std::string, UIElement*> elements;

public:
    UIContainer() = default;
    ~UIContainer();

    void AddElement(std::string id, UIElement* _element);
    void RemoveElement(std::string id);
    UIElement*              GetElement(std::string id);
    const UIElement*        GetElement(std::string id) const;

    void Update();
    void Draw() const;
};

namespace UI
{

    // --------------------
    // --- BUTTON CLASS ---
    // --------------------
    class Button : public UIElement {
    private:
        static Color TINT_PRESS;
        bool hover, press;
        void InitButton();
        void TextureSetup();
        Rectangle hitbox;

        std::function<void()> callbackFunction;

    public:
        Button();
        Button(Vector2 position);
        Button(Vector2 position, float rotation);
        Button(Transform2D _transform);
        Button(Texture2D _texture);
        Button(Texture2D _texture, Vector2 position);
        Button(Texture2D _texture, Vector2 position, float rotation);
        Button(Texture2D _texture, Transform2D _transform);

        bool IsPressed() const;
        bool IsReleased() const;
        bool IsHover() const;

        void DefineOnPressCallback(std::function<void()> callback);

        void Update() override;
        void Draw() const override;
    };

    // -------------------
    // --- PANEL CLASS ---
    // -------------------
    class Panel : public UIElement{
    private:
        Color   col;
        Color   edge_col;
        Vector2 dimensions;
        unsigned int edge_thickness;
    public:
        Panel(Transform2D _transform = {}, Vector2 _dimensions = {0,0}, Color _col = {255, 255, 255, 255}, Color _edge_col = {255, 255, 255, 255}, unsigned int _edge_thickness = 0);

        void Draw() const override;
        void Update() override;
    };

}
#endif // UI_H
