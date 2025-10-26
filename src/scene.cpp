#include "scene.hpp"
#include <iostream>

Scene::Scene()
{
    ui     = nullptr;
}

Scene::~Scene()
{
    delete ui;
    for (auto &obj : objects)
    {
        delete obj.second;
    }
}

void Scene::AddUi(UIContainer *_ui)
{
    ui = _ui;
}

void Scene::AddObject(std::string id, GameObject *_object)
{
    objects.emplace(id, _object);
}

void Scene::AddObjectList(std::map<std::string, GameObject *> _objects)
{
    objects.merge(_objects);
}

GameObject &Scene::GetObject(std::string id)
{
    auto it = objects.find(id);
    if (it != objects.end())
    {
        return *it->second;
    }
    else
    {
        std::cerr << "Error! Object " << id << " not found!";
        static GameObject defaultObj;
        return defaultObj;
    }
}

const GameObject &Scene::GetObject(std::string id) const
{
    auto it = objects.find(id);
    if (it != objects.end())
    {
        return *it->second;
    }
    else
    {
        std::cerr << "Error! Object " << id << " not found!";
        static GameObject defaultObj;
        return defaultObj;
    }
}

void Scene::Draw() const
{
    BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);
            DrawBackground(true, camera);
            for (auto &obj : objects)
            {
                obj.second->Draw();
            }
        EndMode2D();
        if (ui)
            ui->Draw();
    EndDrawing();
}

void Scene::Update()
{
    for (auto &obj : objects)
    {
        obj.second->Update();
    }
    if (ui)
        ui->Update();
}


// =================================
// === SCENE MANAGER SHENANIGANS ===
// =================================
Scene SceneManager::emptyScene = Scene();

SceneManager::SceneManager() : activeScene(&emptyScene)
{
}

SceneManager::~SceneManager()
{
    for (auto &scene : scenes){
        delete scene.second;
    }
}

void SceneManager::AddScene(std::string scene_id, Scene *_scene)
{
    scenes.emplace(scene_id, _scene);
}

void SceneManager::LoadScene(std::string scene_id)
{
    auto it = scenes.find(scene_id);
    if (it == scenes.end())
    {
        activeScene = &emptyScene;
    }
    else
    {
        activeScene = it->second;
    }
}

Scene &SceneManager::GetActiveScene()
{
    return *activeScene;
}

void SceneManager::Draw() const
{
    activeScene->Draw();
}

void SceneManager::Update()
{
    activeScene->Update();
}
