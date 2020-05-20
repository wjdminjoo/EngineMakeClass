#pragma once

#include "InputManager.h"

class GameEngine
{
public:
	GameEngine() = default;

public:
	bool Init();
	bool LoadScene();
	bool LoadResource();


	InputManager& GetInputManager() { return _InputManager; }
	GameObject2D* GetPlayer() { return _Player.get(); }
	Camera2D* GetCamera() { return _Camera.get(); }
	std::vector<std::unique_ptr<GameObject2D>>& GetObject() { return _Object; }

private:
	std::unique_ptr<GameObject2D> _Player;
	std::unique_ptr<Mesh> _QuadMesh;
	std::unique_ptr<Camera2D> _Camera;
	std::vector<std::unique_ptr<GameObject2D>> _Object;

	InputManager _InputManager;
};
