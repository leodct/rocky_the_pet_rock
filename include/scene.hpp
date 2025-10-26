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
    Scene();
    ~Scene();
    
    void AddUi(UIContainer *_ui);
    void AddObject(std::string id, GameObject *_object);
    void AddObjectList(std::map<std::string, GameObject*> _objects);

    GameObject       &GetObject(std::string id);
    const GameObject &GetObject(std::string id) const;

    void Draw() const;
    void Update();
};

class SceneManager {
private:
    std::map<std::string, Scene*> scenes;
    static Scene emptyScene;
    Scene       *activeScene;
public:
    SceneManager();
    ~SceneManager();

    void   AddScene(std::string scene_id, Scene *_scene);
    void   LoadScene(std::string scene_id);
    Scene& GetActiveScene();

    void Draw() const;
    void Update();
};
#endif