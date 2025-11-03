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
    bool enabled;
    void Init();
public:
    UIElement(bool _active = true);
    UIElement(Vector2 position, bool _active = true);
    UIElement(Transform2D _transform, bool _active = true);
    virtual ~UIElement();

    int  GetDrawOrder() const;
    void SetDrawOrder(int _order);

    void ToggleDisplayState();
    void SetDisplayState(bool _active);
    bool GetDisplayState();

    void Enable();
    void Disable();
    void ToggleEnabled();
    bool IsEnabled() const;
    

    virtual void Draw() const = 0;
    virtual void Update() = 0;
};

class UIContainer {
private:
    std::map<std::string, UIElement*> elements;
    int draw_order;
public:
    UIContainer();
    ~UIContainer();

    void AddElement(std::string id, UIElement* _element);
    void RemoveElement(std::string id);
    UIElement*              GetElement(std::string id);
    const UIElement*        GetElement(std::string id) const;

    int  GetDrawOrder() const;
    void SetDrawOrder(int _order);

    void Update();
    void Draw() const;

    void EnableAll();
    void DisableAll();
    void ToggleAll();
    void SetAllVisibilityTo(bool value);
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

    // -------------------
    // --- LABEL CLASS ---
    // -------------------
    class Label : public UIElement{
    public:
        enum class ALIGNMENT { LEFT, MIDDLE, RIGHT };
    protected:
        Color text_col;
        std::string text;
        unsigned int text_size;
        ALIGNMENT alignment;
    public:
        Label(Transform2D _transform = {}, std::string _text = "text", unsigned int _text_size = 1, Color _text_col = BLACK, ALIGNMENT _alignment = ALIGNMENT::LEFT);

        std::string       &GetText();
        const std::string &GetText() const;
        void               SetText(std::string _text);

        unsigned int       GetFontSize() const;
        void               SetFontSize(unsigned int _size);

        ALIGNMENT         GetAlignment() const;
        void               SetAlignment(ALIGNMENT _alignment);

        Color              GetTextColor() const;
        void               SetTextColor(Color color);

        void Update() override;
        void Draw() const override;

    };

    // ------------------------------
    // --- VARIABLE DISPLAY CLASS ---
    // ------------------------------
    template <typename T>
    class VariableDisplay : public Label{
    private:
        const T *variable;
    public:
        VariableDisplay(T *_variable, Transform2D _transform = {}, unsigned int _text_size = 1, Color _text_col = BLACK, ALIGNMENT _alignment = ALIGNMENT::LEFT);
        T   &GetVariable() const;

        void Update() override;
    };

    template <typename T>
    UI::VariableDisplay<T>::VariableDisplay(T *_variable, Transform2D _transform, unsigned int _text_size, Color _text_col, ALIGNMENT _alignment) : variable(_variable)
    {
        transform = _transform;
        text_size = _text_size;
        text_col  = _text_col;
        alignment = _alignment;
    }

    template <typename T>
    inline T &VariableDisplay<T>::GetVariable() const
    {
        return variable;
    }

    template <typename T>
    inline void VariableDisplay<T>::Update()
    {
        text = std::to_string(*variable);
    }
}
#endif // UI_H
