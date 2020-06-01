
#include "Precompiled.h"
#include "Mesh.h"
#include "Transform2D.h"
#include "GameObject2D.h"
#include "Camera2D.h"
#include "Mesh.h"
#include <random>
#include "GameEngine.h"
bool GameEngine::Init(const ScreenPoint& view)
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

	_ViewPortSize = view;

	return true;
}

bool GameEngine::LoadScene()
{
	_ViewPortSize;
	static float initScale = 10.0f;
	std::mt19937 rand(0);
	std::uniform_real_distribution<float> dis(-10000.0f, 10000.0f);


	for (int j = 0; j < 10000; j++) {
		_Object.push_back(std::make_unique<GameObject2D>("Object", _QuadMesh["QuadMesh"].get()));
		_Object[_Object.size() - 1]->GetTransform().SetScale(Vector2::One * initScale);
		_Object[_Object.size() - 1]->GetTransform().SetPosition(Vector2(dis(rand), dis(rand)));
	}
	_Object.push_back(std::make_unique<GameObject2D>("Player", _QuadMesh["QuadMesh"].get()));
	_Object[_Object.size() - 1]->GetTransform().SetScale(Vector2::One * initScale);


	_Camera = std::make_unique<Camera2D>();
	_Camera->SetCameraViewSize(_ViewPortSize);
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

	_QuadMesh["QuadMesh"]->CalcuBound();

	return true;
}

GameObject2D* GameEngine::GameObjectFinder(std::string name)
{
	for (int i = 0; i < _Object.size(); i++) {
		if (name == _Object[i].get()->GetName()) {
			return _Object[i].get();
		}
	}

	return NULL;
}

void GameEngine::SetQuadTree(Rectangle& rect, std::vector<GameObject2D*> object) {
	_Quadtree = std::make_unique<QuadTree>(rect);

	Rectangle rectangle = rect;
	for (int i = 0; i < object.size(); i++) {
		rectangle.Min *= object[i]->GetTransform().GetScale().Max();
		rectangle.Max *= object[i]->GetTransform().GetScale().Max();
		rectangle.Min += object[i]->GetTransform().GetPosition();
		rectangle.Max += object[i]->GetTransform().GetPosition();
		
		_Quadtree->Insert(rectangle, object[i]);
	}

}