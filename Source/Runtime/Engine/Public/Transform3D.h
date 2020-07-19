#pragma once
class Transform3D
{
public:
	Transform3D() = default;

public:

	void SetPosition(const Vector3& InPosition);
	void AddPosition(const Vector3& InDeltaPosition);
	void SetScale(const Vector3& InScale);
	void AddScale(const Vector3& InDeltaScale);
	void SetRotation(const Rotator& InRotator);
	void SetRotation(const Quaternion& InQuaternion);
	void SetRotation(const Vector3& InForwardVector);

	void SetWorldScale(const Vector3& InScale);
	void SetWorldRotation(const Rotator& InRotator);
	void SetWorldRotation(const Quaternion& InQuaternion);
	void SetWorldRotation(const Vector3& InForwardVector);
	void SetWorldPosition(const Vector3& InPosition);

	void AddRotation(Rotator InDeltaDegree);

	void AddWorldPosition(const Vector3& InDeltaPosition);

	void AddWorldRotation(Rotator& InDeltaDegree);


	void AddLocalYawRotation(float InDegree);
	void AddLocalRollRotation(float InDegree);
	void AddLocalPitchRotation(float InDegree);

	Vector3 GetLocalPosition() const { return _Position; }
	Vector3 GetLocalScale() const { return _Scale; }
	Quaternion GetLocalRotation() const { return _Rotation; }

	Vector3 GetWorldPosition() const;
	Vector3 GetWorldScale() const;
	Quaternion GetWorldRotation() const;

	Matrix4x4 GetModelingMatrix() const;
	Matrix4x4 GetInvModelingMatrix() const;
	Matrix4x4 GetWorldModelingMatrix() const;
	Matrix4x4 GetInvWorldModelingMatrix() const;
	bool SetParent(Transform3D* InTransformPtr);
	void Update();

	const Vector3& GetLocalX() const { return _Right; }
	const Vector3& GetLocalY() const { return _Up; }
	const Vector3& GetLocalZ() const { return _Forward; }

	FORCEINLINE Transform3D* GetParent() const { return _ParentPtr; }
	FORCEINLINE Transform3D& GetRoot() {
		Transform3D* current = this;
		Transform3D* parent = nullptr;
		while ((parent = current->GetParent()) != nullptr)
		{
			current = parent;
		}

		return *current;
	}
	FORCEINLINE Transform3D& GetChild(BYTE InIndex) const
	{
		assert(InIndex < _Children.size());
		return *_Children[InIndex];
	}

	FORCEINLINE Transform3D* GetLastChild() const
	{
		return _Children[_Children.size()];
	}
private:
	void CalculateLocalAxis();
	void CalculateLocalAxis(const Vector3& InForwardVector);
	void CalculateMatrix();


	Vector3 _Position = Vector3::Zero;
	Quaternion _Rotation = Quaternion::Identity;
	Vector3 _Scale = Vector3::One;

	Vector3 _WorldPosition = Vector3::Zero;
	Quaternion _WorldRotation = Quaternion::Identity;
	Vector3 _WorldScale = Vector3::One;


	Vector3 _Right = Vector3::UnitX;
	Vector3 _Up = Vector3::UnitY;
	Vector3 _Forward = Vector3::UnitZ;

	Matrix4x4 _LocalTRS = Matrix4x4::Identity;
	Matrix4x4 _TRS = Matrix4x4::Identity;

	friend class GameObject3D;
	friend class Camera3D;

	Transform3D* _ParentPtr = nullptr;
	std::vector<Transform3D*> _Children;
};