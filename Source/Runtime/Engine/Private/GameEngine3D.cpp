#include "Precompiled.h"
#include "Mesh3D.h"
#include "Transform3D.h"
#include "GameObject3D.h"
#include "Camera3D.h"
#include "GameEngine3D.h"
#include <ctime>
#include <random>

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
	/*static const Vector3 bodyOffset(0.f, 0.75f, 0.f);
	static const Vector3 headPivotOffset(0.f, 0.75f, 0.f);
	static const Vector3 headOffset(0.f, 0.5f, 0.f);
	static const Vector3 rHandPivotOffset(-0.75f, 0.5f, 0.f);
	static const Vector3 rHandOffset(0.f, -0.5f, 0.f);
	static const Vector3 lHandPivotOffset(0.75f, 0.5f, 0.f);
	static const Vector3 lHandOffset(0.f, -0.5f, 0.f);
	static const Vector3 rLegPivotOffset(-0.25f, -0.75f, 0.f);
	static const Vector3 rLegOffset(0.f, -0.75f, 0.f);
	static const Vector3 lLegPivotOffset(0.25f, -0.75f, 0.f);
	static const Vector3 lLegOffset(0.f, -0.75f, 0.f);*/

	/*std::random_device random;
	std::mt19937 mt(random());
	std::uniform_real_distribution<float> dist(-500.f, 500.f);
	*/

	//항상 부모를 설정할 때는 설정 후에 TRS를 만지자.
	_Object.push_back(std::make_unique<GameObject3D>("Body", _QuadMesh["Body"].get()));
	_Object[_Object.size() - 1]->GetTransform().SetScale(Vector3::One * 100);
	_Object[_Object.size() - 1]->GetTransform().SetPosition(Vector3::Zero);


	_Object.push_back(std::make_unique<GameObject3D>("Head", _QuadMesh["Head"].get()));
	_Object[_Object.size() - 1]->GetTransform().SetParent(&_Object[0]->GetTransform());
	_Object[_Object.size() - 1]->GetTransform().SetScale(Vector3::One * 100);
	_Object[_Object.size() - 1]->GetTransform().SetPosition(Vector3(0, 125, 0));

	_Object.push_back(std::make_unique<GameObject3D>("RArm", _QuadMesh["ArmLeg"].get()));
	_Object[_Object.size() - 1]->GetTransform().SetParent(&_Object[0]->GetTransform());
	_Object[_Object.size() - 1]->GetTransform().SetScale(Vector3::One * 100);
	_Object[_Object.size() - 1]->GetTransform().SetPosition(Vector3(-75, 75, 0));

	_Object.push_back(std::make_unique<GameObject3D>("LArm", _QuadMesh["ArmLeg"].get()));
	_Object[_Object.size() - 1]->GetTransform().SetParent(&_Object[0]->GetTransform());
	_Object[_Object.size() - 1]->GetTransform().SetScale(Vector3::One * 100);
	_Object[_Object.size() - 1]->GetTransform().SetPosition(Vector3(75, 75, 0));

	_Object.push_back(std::make_unique<GameObject3D>("LLeg", _QuadMesh["ArmLeg"].get()));
	_Object[_Object.size() - 1]->GetTransform().SetParent(&_Object[0]->GetTransform());
	_Object[_Object.size() - 1]->GetTransform().SetScale(Vector3::One * 100);
	_Object[_Object.size() - 1]->GetTransform().SetPosition(Vector3(-25, -75, 0));

	_Object.push_back(std::make_unique<GameObject3D>("RLeg", _QuadMesh["ArmLeg"].get()));
	_Object[_Object.size() - 1]->GetTransform().SetParent(&_Object[0]->GetTransform());
	_Object[_Object.size() - 1]->GetTransform().SetScale(Vector3::One * 100);
	_Object[_Object.size() - 1]->GetTransform().SetPosition(Vector3(25, -75, 0));

	_Camera = std::make_unique<Camera3D>();
	Vector3 _ViewPosition = Vector3(500, 500, -500.0f);
	_Camera->GetTransform().SetPosition(_ViewPosition);
	_Camera->SetLookAtRotation(_Object[0]->GetTransform().GetLocalPosition());
	return true;
}

bool GameEngine3D::LoadResource()
{
	//_QuadMesh["QuadMesh"] = std::make_unique<Mesh3D>();
	static const float cubeHalfSize = 0.5f;
	static const Vector3 bodyScale(0.5f, 0.75f, 0.25f);
	static const Vector3 armlegSize(0.25f, 0.75f, 0.25f);
	//_QuadMesh["QuadMesh"]->_Vertices = {
	//	// Right 
	//	Vector3(1.f, -1.f, 1.f)* cubeHalfSize,
	//	Vector3(1.f, 1.f, 1.f)* cubeHalfSize,
	//	Vector3(1.f, 1.f, -1.f)* cubeHalfSize,
	//	Vector3(1.f, -1.f, -1.f)* cubeHalfSize,
	//	// Front
	//	Vector3(-1.f, -1.f, 1.f)* cubeHalfSize,
	//	Vector3(-1.f, 1.f, 1.f)* cubeHalfSize,
	//	Vector3(1.f, 1.f, 1.f)* cubeHalfSize,
	//	Vector3(1.f, -1.f, 1.f)* cubeHalfSize,
	//	// Back
	//	Vector3(1.f, -1.f, -1.f)* cubeHalfSize,
	//	Vector3(1.f, 1.f, -1.f)* cubeHalfSize,
	//	Vector3(-1.f, 1.f, -1.f)* cubeHalfSize,
	//	Vector3(-1.f, -1.f, -1.f)* cubeHalfSize,
	//	// Left
	//	Vector3(-1.f, -1.f, -1.f)* cubeHalfSize,
	//	Vector3(-1.f, 1.f, -1.f)* cubeHalfSize,
	//	Vector3(-1.f, 1.f, 1.f)* cubeHalfSize,
	//	Vector3(-1.f, -1.f, 1.f)* cubeHalfSize,
	//	// Top
	//	Vector3(1.f, 1.f, 1.f)* cubeHalfSize,
	//	Vector3(-1.f, 1.f, 1.f)* cubeHalfSize,
	//	Vector3(-1.f, 1.f, -1.f)* cubeHalfSize,
	//	Vector3(1.f, 1.f, -1.f)* cubeHalfSize,
	//	// Bottom
	//	Vector3(-1.f, -1.f, 1.f)* cubeHalfSize,
	//	Vector3(1.f, -1.f, 1.f)* cubeHalfSize,
	//	Vector3(1.f, -1.f, -1.f)* cubeHalfSize,
	//	Vector3(-1.f, -1.f, -1.f)* cubeHalfSize
	//};

	//_QuadMesh["QuadMesh"]->_Indices = {
	//	0, 2, 1, 0, 3, 2,
	//	4, 6, 5, 4, 7, 6,
	//	8, 10, 9, 8, 11, 10,
	//	12, 14, 13, 12, 15, 14,
	//	16, 18, 17, 16, 19, 18,
	//	20, 22, 21, 20, 23, 22
	//};

	//static const Vector3 bodyScale(0.5f, 0.75f, 0.25f);
	//static const Vector3 armlegSize(0.25f, 0.75f, 0.25f);

	_QuadMesh["Head"] = std::make_unique<Mesh3D>();
	_QuadMesh["Body"] = std::make_unique<Mesh3D>();
	_QuadMesh["ArmLeg"] = std::make_unique<Mesh3D>();

	_QuadMesh["Head"]->_Vertices = {
		// Right 
		Vector3(-1.f, -1.f, -1.f) * cubeHalfSize, 
		Vector3(-1.f, -1.f, 1.f) * cubeHalfSize, 
		Vector3(-1.f, 1.f, 1.f) * cubeHalfSize, 
		Vector3(-1.f, 1.f, -1.f) * cubeHalfSize,
		// Front
		Vector3(-1.f, -1.f, 1.f) * cubeHalfSize, 
		Vector3(-1.f, 1.f, 1.f) * cubeHalfSize, 
		Vector3(1.f, 1.f, 1.f) * cubeHalfSize, 
		Vector3(1.f, -1.f, 1.f) * cubeHalfSize,
		// Back
		Vector3(-1.f, -1.f, -1.f) * cubeHalfSize, 
		Vector3(-1.f, 1.f, -1.f) * cubeHalfSize, 
		Vector3(1.f, 1.f, -1.f) * cubeHalfSize, 
		Vector3(1.f, -1.f, -1.f) * cubeHalfSize,
		// Left
		Vector3(1.f, -1.f, -1.f) * cubeHalfSize, 
		Vector3(1.f, -1.f, 1.f) * cubeHalfSize, 
		Vector3(1.f, 1.f, 1.f) * cubeHalfSize, 
		Vector3(1.f, 1.f, -1.f) * cubeHalfSize,
		// Top
		Vector3(-1.f, 1.f, -1.f) * cubeHalfSize, 
		Vector3(1.f, 1.f, -1.f) * cubeHalfSize, 
		Vector3(1.f, 1.f, 1.f) * cubeHalfSize,
		Vector3(-1.f, 1.f, 1.f) * cubeHalfSize,
		// Bottom
		Vector3(-1.f, -1.f, -1.f) * cubeHalfSize, 
		Vector3(1.f, -1.f, -1.f) * cubeHalfSize, 
		Vector3(1.f, -1.f, 1.f) * cubeHalfSize,
		Vector3(-1.f, -1.f, 1.f) * cubeHalfSize
	};

	_QuadMesh["Head"]->_Indices = {
		0, 1, 2, 0, 2, 3, // Right
		4, 6, 5, 4, 7, 6, // Front
		8, 9, 10, 8, 10, 11, // Back
		12, 14, 13, 12, 15, 14, // Left
		16, 18, 17, 16, 19, 18, // Top
		20, 21, 22, 20, 22, 23  // Bottom
	};

	_QuadMesh["Body"]->_Vertices = {
		// Right 
		Vector3(-1.f, -1.f, -1.f) * bodyScale,
		Vector3(-1.f, -1.f, 1.f) * bodyScale, 
		Vector3(-1.f, 1.f, 1.f) * bodyScale,
		Vector3(-1.f, 1.f, -1.f) * bodyScale,
		// Front
		Vector3(-1.f, -1.f, 1.f) * bodyScale, 
		Vector3(-1.f, 1.f, 1.f) * bodyScale,
		Vector3(1.f, 1.f, 1.f) * bodyScale,
		Vector3(1.f, -1.f, 1.f) * bodyScale,
		// Back
		Vector3(-1.f, -1.f, -1.f) * bodyScale,
		Vector3(-1.f, 1.f, -1.f) * bodyScale, 
		Vector3(1.f, 1.f, -1.f) * bodyScale,
		Vector3(1.f, -1.f, -1.f) * bodyScale,
		// Left
		Vector3(1.f, -1.f, -1.f) * bodyScale, 
		Vector3(1.f, -1.f, 1.f) * bodyScale,
		Vector3(1.f, 1.f, 1.f) * bodyScale,
		Vector3(1.f, 1.f, -1.f) * bodyScale,
		// Top
		Vector3(-1.f, 1.f, -1.f) * bodyScale,
		Vector3(1.f, 1.f, -1.f) * bodyScale, 
		Vector3(1.f, 1.f, 1.f) * bodyScale,
		Vector3(-1.f, 1.f, 1.f) * bodyScale,
		// Bottom
		Vector3(-1.f, -1.f, -1.f) * bodyScale, 
		Vector3(1.f, -1.f, -1.f) * bodyScale, 
		Vector3(1.f, -1.f, 1.f) * bodyScale,
		Vector3(-1.f, -1.f, 1.f) * bodyScale
	};

	_QuadMesh["Body"].get()->_Indices = _QuadMesh["Head"].get()->_Indices;

	_QuadMesh["ArmLeg"].get()->_Vertices = {
		// Right 
		Vector3(-1.f, -1.f, -1.f) * armlegSize,
		Vector3(-1.f, -1.f, 1.f) * armlegSize, 
		Vector3(-1.f, 1.f, 1.f) * armlegSize,
		Vector3(-1.f, 1.f, -1.f) * armlegSize,
		// Front
		Vector3(-1.f, -1.f, 1.f) * armlegSize, 
		Vector3(-1.f, 1.f, 1.f) * armlegSize,
		Vector3(1.f, 1.f, 1.f) * armlegSize, 
		Vector3(1.f, -1.f, 1.f) * armlegSize,
		// Back
		Vector3(-1.f, -1.f, -1.f) * armlegSize,
		Vector3(-1.f, 1.f, -1.f) * armlegSize, 
		Vector3(1.f, 1.f, -1.f) * armlegSize,
		Vector3(1.f, -1.f, -1.f) * armlegSize,
		// Left
		Vector3(1.f, -1.f, -1.f) * armlegSize,
		Vector3(1.f, -1.f, 1.f) * armlegSize, 
		Vector3(1.f, 1.f, 1.f) * armlegSize,
		Vector3(1.f, 1.f, -1.f) * armlegSize,
		// Top
		Vector3(-1.f, 1.f, -1.f) * armlegSize,
		Vector3(1.f, 1.f, -1.f) * armlegSize,
		Vector3(1.f, 1.f, 1.f) * armlegSize, 
		Vector3(-1.f, 1.f, 1.f) * armlegSize,
		// Bottom
		Vector3(-1.f, -1.f, -1.f) * armlegSize, 
		Vector3(1.f, -1.f, -1.f) * armlegSize,
		Vector3(1.f, -1.f, 1.f) * armlegSize,
		Vector3(-1.f, -1.f, 1.f) * armlegSize
	};

	_QuadMesh["ArmLeg"].get()->_Indices = _QuadMesh["Head"].get()->_Indices;

	for (auto&& i : _QuadMesh["ArmLeg"]->_Vertices)
		i = Vector3(i.X, i.Y - armlegSize.Y, i.Z);

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

