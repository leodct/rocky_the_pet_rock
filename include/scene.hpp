#ifndef SCENE_H
#define SCENE_H

#include "globals.hxx"
#include "UI.hpp"
#include "gameObject.hxx"

class Scene {
private:
    UIContainer *ui;
    std::map<std::string, GameObject*> objects;
public:
    Scene() = default;
    ~Scene();
    
    void AddUi(UIContainer *_ui);
    void AddObject(std::string id, GameObject *_object);
    void AddObjectList(std::map<std::string, GameObject*> _objects);

    GameObject       &GetObject(std::string id);
    const GameObject &GetObject(std::string id) const;

    void Draw() const;
    void Update();
};
#endif