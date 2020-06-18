#include "Precompiled.h"
#include "Mesh2D.h"
#include "Transform2D.h"
#include "GameObject2D.h"
#include "Camera2D.h"
#include "GameEngine2D.h"

bool GameEngine2D::Init(const ScreenPoint& view)
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

bool GameEngine2D::LoadScene()
{
	_Object.push_back(std::make_unique<GameObject2D>("Player", _QuadMesh["QuadMesh"].get()));
	_Object.push_back(std::make_unique<GameObject2D>("Child", _QuadMesh["QuadMesh"].get()));
	_Object.push_back(std::make_unique<GameObject2D>("ChildTwo", _QuadMesh["QuadMesh"].get()));

	_Object[0]->GetTransform().SetLocalScale(Vector2::One * 100);

	_Object[1]->GetTransform().SetLocalScale(Vector2::One * 50);
	_Object[1]->GetTransform().SetLocalPosition(Vector2(350, 0));

	_Object[2]->GetTransform().SetLocalScale(Vector2::One * 10);
	_Object[2]->GetTransform().SetLocalPosition(Vector2(6000, 0));

	// 분명 Child에 넣는데 Player의 주소를 갖고있는다..
	_Object[1]->GetTransform().SetParent(&_Object[0]->GetTransform());
	_Object[2]->GetTransform().SetParent(&_Object[1]->GetTransform());
	

	_Camera = std::make_unique<Camera2D>();
	_Camera->SetCameraViewSize(_ViewPortSize);
	return true;
}

bool GameEngine2D::LoadResource()
{
	_QuadMesh["QuadMesh"] = std::make_unique<Mesh2D>();

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

GameObject2D* GameEngine2D::GameObjectFinder(std::string name)
{
	for (int i = 0; i < _Object.size(); i++) {
		if (name == _Object[i].get()->GetName()) {
			return _Object[i].get();
		}
	}

	return NULL;
}

void GameEngine2D::SetQuadTree(Rectangle& rect, std::vector<GameObject2D*> object) {
	_Quadtree = std::make_unique<QuadTree>(rect);

	Rectangle rectangle = rect;
	for (int i = 0; i < object.size(); i++) {
		rectangle.Min *= object[i]->GetTransform().GetLocalScale().Max();
		rectangle.Max *= object[i]->GetTransform().GetLocalScale().Max();
		rectangle.Min += object[i]->GetTransform().GetLocalScale();
		rectangle.Max += object[i]->GetTransform().GetLocalScale();
		
		_Quadtree->Insert(rectangle, object[i]);
	}

}