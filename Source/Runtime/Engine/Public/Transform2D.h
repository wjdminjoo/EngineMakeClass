#pragma once
#include <cmath>
class Transform2D
{
public:
	Transform2D() = default;

public:
	/*void SetPosition(const Vector2& InPosition) { _Position = InPosition; }
	void AddPosition(const Vector2& InDeltaPosition) { _Position += InDeltaPosition; }
	void SetScale(const Vector2& InScale) { _Scale = InScale; }
	void SetRotation(float InDegree) { _Rotation = InDegree; CalculateLocalAxis(); }
	void AddRotation(float InDegree) { _Rotation += InDegree; CalculateLocalAxis(); }*/
	//Vector2 GetPosition() const { return _Position; }
	//Vector2 GetScale() const { return _Scale; }
	//float GetRotation() const { return _Rotation; }
	
	Matrix3x3 GetModelingMatrix() const;
	Matrix3x3 GetInvWorldModelingMatrix() const;
	Matrix3x3 GetWorldModelingMatrix() const;
	const Vector2& GetLocalX() const { return Right; }
	const Vector2& GetLocalY() const { return Up; }

	//TEST ParentPtr
	std::vector<Transform2D*> Find(Transform2D* it);
	Transform2D* End();
	Transform2D* Begin();

	FORCEINLINE Transform2D* GetParent() const
	{
		return _ParentPtr;
	}

	FORCEINLINE Transform2D& GetRoot()
	{
		Transform2D* current = this;
		Transform2D* parent = nullptr;
		while ((parent = current->GetParent()) != nullptr)
		{
			current = parent;
		}

		return *current;
	}

	FORCEINLINE Transform2D& GetChild(BYTE InIndex) const
	{
		assert(InIndex < _Children.size());
		return *_Children[InIndex];
	}

	FORCEINLINE Transform2D* GetLastChild() const
	{
		return _Children[_Children.size()];
	}


	bool SetParent(Transform2D* InTransformPtr);
	void Update();
	void CalculateMatrices();

	// FORCEINLINE을 update()때문에 지움
	// 자동 생성이 읽기 상태라고 안됨. 
	// 직접 써줘서 사용했음.
	void SetLocalPosition(const Vector2& InPosition) { 
		//if (_ParentPtr == nullptr)
		_Position = InPosition;
		//else
		//	_Position = _ParentPtr->GetLocalPosition() + InPosition;
		Update(); 
	}
	void AddLocalPosition(const Vector2& InDeltaPosition) { 
		//if (_ParentPtr == nullptr)
		_Position += InDeltaPosition;
		//else
		//	_Position += _ParentPtr->GetLocalPosition() + InDeltaPosition;
		Update(); 
	}
	void SetLocalScale(const Vector2& InScale) { _Scale = InScale; Update(); }
	void AddLocalScale(const Vector2& InScale) { _Scale += InScale; Update(); }
	void SetLocalRotation(float InDegree) { _Rotation = InDegree; Update(); }
	void AddLocalRotation(float InDeltaDegree){ _Rotation += InDeltaDegree; Update(); }

	void SetWorldScale(const Vector2& InScale);
	void AddWorldScale(const Vector2& InScale);
	void SetWorldRotation(float InDegree);
	void AddWorldRotation(float InDeltaDegree);
	void SetWorldPosition(const Vector2& InPosition);
	void AddWorldPosition(const Vector2& InDeltaPosition);

	Vector2 GetLocalPosition() const { 
		/*if (GetParent() != nullptr)
			return _Position = _Position - GetParent()->GetLocalPosition();*/

		return _Position; 
	}
	Vector2 GetLocalScale() const {
		/*if (GetParent() != nullptr)
			return _Scale = GetParent()->GetLocalScale() - _Scale;*/

		return _Scale; 
	}
	float GetLocalRotation() const {
		/*if (GetParent() != nullptr)
			return _Rotation = _Rotation - GetParent()->GetLocalRotation();*/

		return _Rotation; 
	}

	FORCEINLINE Vector2 GetWorldPosition() const{ 
		return _WorldPosition;
	}
	FORCEINLINE Vector2 GetWorldScale() const {
		return _WorldScale;
	}
	FORCEINLINE float GetWorldRotation() const {
		return _WorldRotation; 
	}

	FORCEINLINE Matrix3x3 GetPositionMatrix(float a, float b) {
		return Matrix3x3(1, 0, a, 0, 1, b, 0, 0, 1);
	}

	FORCEINLINE Matrix3x3 GetRotationMatrix(float a) {
		return Matrix3x3(std::cos(a), std::sin(-a), 0, std::sin(a), std::cos(a), 0, 0, 0, 1);
	}

	FORCEINLINE Matrix3x3 GetScaleMatrix(float a, float b) {
		return Matrix3x3(a, 0, 0, 0, b, 0, 0, 0, 1);
	}

	
private:
	void CalculateLocalAxis();

	Vector2 _Position = Vector2::Zero;
	float _Rotation = 0.f;
	Vector2 _Scale = Vector2::One;

	Vector2 Right = Vector2::UnitX;
	Vector2 Up = Vector2::UnitY;

	friend class GameObject2D;

	//TEST ParentPtr
	Transform2D* _ParentPtr = nullptr;
	std::vector<Transform2D*> _Children;
	Vector2 _WorldPosition = Vector2::Zero;
	Vector2 _WorldScale = Vector2::One;
	float _WorldRotation = 0.f;
	Matrix3x3 _WorldTRS;
};