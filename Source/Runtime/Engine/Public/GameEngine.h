#pragma once

#include "InputManager.h"

class GameEngine
{
public:
	GameEngine() = default;

	bool Init();
	bool LoadScene();
	bool LoadResource();


	InputManager& GetInputManager() { return _InputManager; }
	Camera2D* GetCamera() { return _Camera.get(); }
	std::vector<std::unique_ptr<GameObject2D>>& GetObject() { return _Object; }

	GameObject2D* GameObjectFinder(std::string name);
private:


	std::unordered_map<std::string, std::unique_ptr<Mesh>> _QuadMesh;
	std::unique_ptr<Camera2D> _Camera;
	std::vector<std::unique_ptr<GameObject2D>> _Object;

	InputManager _InputManager;
};
