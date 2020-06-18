#include "Precompiled.h"
#include "Mesh3D.h"
#include "Transform3D.h"
#include "GameObject3D.h"
#include "Camera3D.h"
#include "GameEngine3D.h"

bool GameEngine3D::Init(const ScreenPoint& view)
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

bool GameEngine3D::LoadScene()
{
	_Object.push_back(std::make_unique<GameObject3D>("Player", _QuadMesh["QuadMesh"].get()));
	_Object[0]->GetTransform().SetScale(Vector3::One * 100);
	_Object[0]->GetTransform().SetPosition(Vector3::Zero);
	

	_Camera = std::make_unique<Camera3D>();
	Vector3 _ViewPosition = Vector3(00.0f, 00.0f, -500.0f);
	_Camera->GetTransform().SetPosition(_ViewPosition);
	_Camera->SetLookAtRotation(_Object[0]->GetTransform().GetLocalPosition());
	return true;
}

bool GameEngine3D::LoadResource()
{
	_QuadMesh["QuadMesh"] = std::make_unique<Mesh3D>();
	static const float cubeHalfSize = 0.5f;

	_QuadMesh["QuadMesh"]->_Vertices = {
		// Right 
		Vector3(1.f, -1.f, 1.f)* cubeHalfSize,
		Vector3(1.f, 1.f, 1.f)* cubeHalfSize,
		Vector3(1.f, 1.f, -1.f)* cubeHalfSize,
		Vector3(1.f, -1.f, -1.f)* cubeHalfSize,
		// Front
		Vector3(-1.f, -1.f, 1.f)* cubeHalfSize,
		Vector3(-1.f, 1.f, 1.f)* cubeHalfSize,
		Vector3(1.f, 1.f, 1.f)* cubeHalfSize,
		Vector3(1.f, -1.f, 1.f)* cubeHalfSize,
		// Back
		Vector3(1.f, -1.f, -1.f)* cubeHalfSize,
		Vector3(1.f, 1.f, -1.f)* cubeHalfSize,
		Vector3(-1.f, 1.f, -1.f)* cubeHalfSize,
		Vector3(-1.f, -1.f, -1.f)* cubeHalfSize,
		// Left
		Vector3(-1.f, -1.f, -1.f)* cubeHalfSize,
		Vector3(-1.f, 1.f, -1.f)* cubeHalfSize,
		Vector3(-1.f, 1.f, 1.f)* cubeHalfSize,
		Vector3(-1.f, -1.f, 1.f)* cubeHalfSize,
		// Top
		Vector3(1.f, 1.f, 1.f)* cubeHalfSize,
		Vector3(-1.f, 1.f, 1.f)* cubeHalfSize,
		Vector3(-1.f, 1.f, -1.f)* cubeHalfSize,
		Vector3(1.f, 1.f, -1.f)* cubeHalfSize,
		// Bottom
		Vector3(-1.f, -1.f, 1.f)* cubeHalfSize,
		Vector3(1.f, -1.f, 1.f)* cubeHalfSize,
		Vector3(1.f, -1.f, -1.f)* cubeHalfSize,
		Vector3(-1.f, -1.f, -1.f)* cubeHalfSize
	};

	_QuadMesh["QuadMesh"]->_Indices = {
		0, 2, 1, 0, 3, 2,
		4, 6, 5, 4, 7, 6,
		8, 10, 9, 8, 11, 10,
		12, 14, 13, 12, 15, 14,
		16, 18, 17, 16, 19, 18,
		20, 22, 21, 20, 23, 22
	};


	return true;
}

GameObject3D* GameEngine3D::GameObjectFinder(std::string name)
{
	for (int i = 0; i < _Object.size(); i++) {
		if (name == _Object[i].get()->GetName()) {
			return _Object[i].get();
		}
	}

	return NULL;
}

