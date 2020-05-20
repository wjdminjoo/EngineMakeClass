#pragma once


class GameObject2D {
public:
	GameObject2D() = default;
	GameObject2D(const Mesh* InMeshPtr) : _MeshPtr(InMeshPtr) { }
	Transform2D& GetTransform() { return _Transform; }
	const Mesh* GetMesh() { return _MeshPtr; }
	bool HasMesh() const { return (_MeshPtr != nullptr); }

protected:
	Transform2D _Transform;
	const Mesh* _MeshPtr = nullptr;
};