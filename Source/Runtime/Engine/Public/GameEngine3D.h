#pragma once

#include "InputManager.h"
#include "RectAngle.h"
class GameEngine3D
{
public:
	GameEngine3D() = default;

	bool Init(const ScreenPoint& view);
	bool LoadScene();
	bool LoadResource();


	InputManager& GetInputManager() { return _InputManager; }
	Camera3D* GetCamera() { return _Camera.get(); }
	std::vector<std::unique_ptr<GameObject3D>>& GetObject() { return _Object; }
	GameObject3D* GameObjectFinder(std::string name);
private:


	std::unordered_map<std::string, std::unique_ptr<Mesh3D>> _QuadMesh;
	std::unique_ptr<Camera3D> _Camera;
	std::vector<std::unique_ptr<GameObject3D>> _Object;


	InputManager _InputManager;

	ScreenPoint _ViewPortSize;
};
