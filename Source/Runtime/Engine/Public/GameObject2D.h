#pragma once


class GameObject2D {
public:
	GameObject2D() = default;
	GameObject2D(std::string name,const Mesh* InMeshPtr) : _ObjectName(name), _MeshPtr(InMeshPtr) { }
	Transform2D& GetTransform() { return _Transform; }
	const Mesh* GetMesh() { return _MeshPtr; }
	bool HasMesh() const { return (_MeshPtr != nullptr); }
	std::string GetName() { return _ObjectName; }

protected:
	Transform2D _Transform;
	const Mesh* _MeshPtr = nullptr;

	std::string _ObjectName;
};