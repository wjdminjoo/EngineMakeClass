#pragma once

#include "InputManager.h"
#include "RectAngle.h"
class GameEngine2D
{
public:
	GameEngine2D() = default;

	bool Init(const ScreenPoint& view);
	bool LoadScene();
	bool LoadResource();


	InputManager& GetInputManager() { return _InputManager; }
	Camera2D* GetCamera() { return _Camera.get(); }
	std::vector<std::unique_ptr<GameObject2D>>& GetObject() { return _Object; }
	QuadTree* GetQuadTree() { return _Quadtree.get(); }
	void SetQuadTree(Rectangle& rect, std::vector<GameObject2D*> object);
	GameObject2D* GameObjectFinder(std::string name);
private:


	std::unordered_map<std::string, std::unique_ptr<Mesh2D>> _QuadMesh;
	std::unique_ptr<Camera2D> _Camera;
	std::vector<std::unique_ptr<GameObject2D>> _Object;
	std::unique_ptr<QuadTree> _Quadtree;


	InputManager _InputManager;

	ScreenPoint _ViewPortSize;
};
