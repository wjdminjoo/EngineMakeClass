
#include "Precompiled.h"
#include "Mesh.h"
#include "Transform2D.h"
#include "GameObject2D.h"
#include "Camera2D.h"
#include "GameEngine.h"
bool GameEngine::Init()
{
	if (!_InputManager.GetXAxis || !_InputManager.GetYAxis || !_InputManager.SpacePressed)
	{
		return false;
	}

	if (!LoadResource())
	{
		return false;
	}

	if (!LoadScene())
	{
		return false;
	}

	return true;
}

bool GameEngine::LoadScene()
{
	static float initScale = 100.0f;

	for (int i = -10; i < 10; i++) {
		for (int j = -10; j < 10; j++) {
			_Object.push_back(std::make_unique<GameObject2D>("Object", _QuadMesh["QuadMesh"].get()));
			_Object[_Object.size() - 1]->GetTransform().SetScale(Vector2::One * initScale);
			_Object[_Object.size() - 1]->GetTransform().SetPosition(Vector2(initScale, j * initScale + 100));
		}
		_Object[_Object.size() - 1]->GetTransform().SetPosition(Vector2(i * initScale + 100, initScale));
	}
	_Object.push_back(std::make_unique<GameObject2D>("Player", _QuadMesh["QuadMesh"].get()));
	_Object[_Object.size() - 1]->GetTransform().SetScale(Vector2::One * initScale);
	
	_Camera = std::make_unique<Camera2D>();

	return true;
}

bool GameEngine::LoadResource()
{
	_QuadMesh["QuadMesh"] = std::make_unique<Mesh>();

	static float squareHalfSize = 0.5f;
	static int vertexCount = 4;
	static int triangleCount = 2;
	static int indexCount = triangleCount * 3;

	_QuadMesh["QuadMesh"]->_Vertices = {
		Vector2(-squareHalfSize, -squareHalfSize),
		Vector2(-squareHalfSize, squareHalfSize),
		Vector2(squareHalfSize, squareHalfSize),
		Vector2(squareHalfSize, -squareHalfSize)
	};

	_QuadMesh["QuadMesh"]->_Indices = {
		0, 2, 1,
		0, 3, 2
	};


	return true;
}

GameObject2D* GameEngine::GameObjectFinder(std::string name)
{
	for (int i = 0; i < _Object.size(); i++) {
		if (name ==  _Object[i].get()->GetName()) {
			return _Object[i].get();
		}
	}

	return NULL;
}

