#include "scene.hpp"
#include <iostream>

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
            DrawBackground(true);
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
