#pragma once


class GameObject3D {
public:
	GameObject3D() = default;
	GameObject3D(std::string name,const Mesh3D* InMeshPtr) : _ObjectName(name), _MeshPtr(InMeshPtr) { }
	Transform3D& GetTransform() { return _Transform; }
	const Mesh3D* GetMesh() { return _MeshPtr; }
	bool HasMesh() const { return (_MeshPtr != nullptr); }
	std::string GetName() { return _ObjectName; }
protected:
	Transform3D _Transform;
	const Mesh3D* _MeshPtr = nullptr;

	std::string _ObjectName;
};