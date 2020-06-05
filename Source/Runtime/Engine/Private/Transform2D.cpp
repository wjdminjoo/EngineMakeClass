#include "Precompiled.h"
#include "Transform2D.h"
#include <assert.h>

Matrix3x3 Transform2D::GetModelingMatrix() const
{
	Matrix3x3 translateMat(Vector3::UnitX, Vector3::UnitY, Vector3(_Position));

	Matrix3x3 rotationMat(
		Vector3(Right, false),
		Vector3(Up, false),
		Vector3::UnitZ);

	Matrix3x3 scaleMat(Vector3::UnitX * _Scale.X, 
		Vector3::UnitY * _Scale.Y,
		Vector3::UnitZ);
	return translateMat * rotationMat * scaleMat;
}

Matrix3x3 Transform2D::GetInvWorldModelingMatrix() const
{
	Matrix3x3 translateMat(Vector3::UnitX, Vector3::UnitY, Vector3(-_WorldPosition));

	Matrix3x3 rotationMat(
		Vector3(Right, false),
		Vector3(Up, false),
		Vector3::UnitZ);
	rotationMat = rotationMat.Tranpose();

	Matrix3x3 scaleMat(Vector3::UnitX * (1.0f / _WorldScale.X),
		Vector3::UnitY * (1.0f / _WorldScale.Y),
		Vector3::UnitZ);

	Matrix3x3 result = scaleMat * rotationMat * translateMat;

	if (_ParentPtr)
		return result = _ParentPtr->GetInvWorldModelingMatrix() * result;

	return result;
}

Matrix3x3 Transform2D::GetWorldModelingMatrix() const
{
	Matrix3x3 translateMat(Vector3::UnitX, Vector3::UnitY, Vector3(_WorldPosition));

	Matrix3x3 rotationMat(
		Vector3(Right, false),
		Vector3(Up, false),
		Vector3::UnitZ);

	Matrix3x3 scaleMat(Vector3::UnitX * _WorldScale.X,
		Vector3::UnitY * _WorldScale.Y,
		Vector3::UnitZ);

	Matrix3x3 result = translateMat * rotationMat * scaleMat;

	return result;
}

std::vector<Transform2D*> Transform2D::Find(Transform2D* it)
{
	for (int i = 0; i < _Children.size(); i++) {
		if(_Children[i] == it){
			return _Children;
		}
	}
	return _Children;
}

Transform2D* Transform2D::End()
{
	if (_Children.size() != 0)
		return _Children.back();
}

Transform2D* Transform2D::Begin()
{
	if(_Children.size() != 0)
		return _Children.at(0);
}

void Transform2D::SetWorldScale(const Vector2& InScale) {
	_WorldScale = InScale;
	if (_ParentPtr == nullptr)
		_Scale = _WorldScale;
	else
		_Scale = Vector2(
			(1.0f / _ParentPtr->_WorldScale.X) * _WorldScale.X,
			(1.0f / _ParentPtr->_WorldScale.Y) * _WorldScale.Y);

	Update();
}
void Transform2D::AddWorldScale(const Vector2& InScale) {
	_WorldScale += InScale;
	if (_ParentPtr == nullptr)
		_Scale = _WorldScale;
	else {
		_Scale = Vector2(
			(1.0f / _ParentPtr->_WorldScale.X) + _WorldScale.X, 
			(1.0f * _ParentPtr->_WorldScale.Y) + _WorldScale.Y);
	}

	Update();
}
void Transform2D::SetWorldRotation(float InDegree) {
	_WorldRotation = InDegree;
	if (_ParentPtr == nullptr)
		_Rotation = _WorldRotation;
	else
		_Rotation = _WorldRotation - _ParentPtr->_WorldRotation;

	Update();
}

void Transform2D::AddWorldRotation(float InDegree) {
	_WorldRotation += InDegree;
	if (_ParentPtr == nullptr)
		_Rotation = _WorldRotation;
	else
		_Rotation = _WorldRotation - _ParentPtr->_WorldRotation;

	Update();
}

void Transform2D::SetWorldPosition(const Vector2& InPosition) {
	_WorldPosition = InPosition;
	if (_ParentPtr == nullptr)
		_Position = _WorldPosition;
	else
		_Position = _ParentPtr->_WorldPosition + _WorldPosition;

	Update();
}

void Transform2D::AddWorldPosition(const Vector2& InPosition) {
	_WorldPosition += InPosition;
	if (_ParentPtr == nullptr)
		_Position = _WorldPosition;
	else
		_Position = _ParentPtr->_WorldPosition + _WorldPosition;

	Update();
}



void Transform2D::CalculateLocalAxis()
{
	float sin, cos;
	Math::GetSinCos(sin, cos, _Rotation);

	Right = Vector2(cos, sin);
	Up = Vector2(-sin, cos);
}

bool Transform2D::SetParent(Transform2D* InTransformPtr)
{
	// ���� ������ �̹� �����Ǿ� �ִ��� üũ
	for (Transform2D* current = InTransformPtr; current != nullptr; current = current->GetParent())
	{
		if (this == current)
			return false;
	}

	// �̹� �θ� �ִ� ��� �ش� �θ��� �ڽ� ��Ͽ��� �ڽ��� ����.
	if (_ParentPtr != nullptr)
	{
		auto it = std::find(_Children.begin(), _Children.end(), this);
		//std::vector<Transform2D*> it = parent->Find(this);
		assert(it != _ParentPtr->_Children.end());
		_ParentPtr->_Children.erase(it);
		_ParentPtr = nullptr;
	}

	// ���ο� �θ� �ڽ��� ����ϰ� �θ�� ���
	InTransformPtr->_Children.push_back(this);
	_ParentPtr = InTransformPtr;

	// �θ�κ��� ���� ������ ����
	// Scale
	_Scale = Vector2(
		(1.0f / _ParentPtr->_WorldScale.X) * _Scale.X,
		(1.0f / _ParentPtr->_WorldScale.Y) * _Scale.Y); // ���� ���� ������
	// Rotation
	_Rotation = _ParentPtr->_WorldRotation - _Rotation;
	//float invRotation = _ParentPtr->_WorldRotation;
	//_Rotation = invRotation + _Rotation; // ���� ���� ȸ��
	// Position
	// �θ� ��ġ�κ����� ����� �Ÿ�
	// �̸� Inv ȸ����ŭ ȸ���� �̵� ���� ����
	// ȸ���� �̵� ���Ϳ� Inv������ ����  ( ���� ���� ��ġ )
	_Position = _ParentPtr->GetInvWorldModelingMatrix() * _Position;

	Update(); // ������Ʈ

	return true;
}

void Transform2D::Update()
{
	CalculateLocalAxis(); // �ڽ��� ���� �� ������ ������
	CalculateMatrices(); // ����� �̸� ���صα�

	// �ڽĵ鵵 ��� ������ �����Ƿ� ��������� ��� ����
	for (auto&& it = _Children.begin(); it != _Children.end(); ++it)
	{
		(*it)->Update(); // World ������ ����
	}
}

void Transform2D::CalculateMatrices()
{
	if (_ParentPtr == nullptr)
	{
		_WorldPosition = GetLocalPosition();
		_WorldRotation = GetLocalRotation();
		_WorldScale = GetLocalScale();
		_WorldTRS = GetModelingMatrix();  // ��Ʈ�� ��� ���� TRS�� ���� TRS�� ���� 
	}
	else
	{
		_WorldPosition = _ParentPtr->GetWorldModelingMatrix() * _Position;
		_WorldRotation = _ParentPtr->GetWorldRotation() + _Rotation;
		_WorldScale = _ParentPtr->GetWorldScale() * _Scale;
		_WorldTRS = _ParentPtr->GetWorldModelingMatrix() * GetModelingMatrix(); // �θ��� ���� TRS�� ���� TRS�� ���Ͽ� ���� TRS ����� ����
	}
}
