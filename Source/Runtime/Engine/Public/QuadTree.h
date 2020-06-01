#pragma once
#include "Precompiled.h"
#include <memory>
#include <limits>
enum SECTOR {
	NW,
	NE,
	SW,
	SE
};
class QuadTree {
public:
	QuadTree() = default;
	QuadTree(const Rectangle& rectangle) : rect(rectangle) {}
	QuadTree(const Vector2& inMinVec,
		const Vector2& inMaxVec)
		: rect(Rectangle(inMinVec, inMaxVec)) { }
	~QuadTree() {
		for (int i = NW; i < SE; ++i)
		{
			if (children[i] != nullptr)
			{
				delete children[i];
				children[i] = nullptr;
			}
		}
	};

	void Insert(const Rectangle& rect, GameObject2D* object);
	bool GetSectorObject(const Rectangle& rect, std::vector<GameObject2D*>& object);
	void GetAllObject(std::vector<GameObject2D*>& object);

	char InRect(const Rectangle& rect);
	Rectangle rect;

private:
	std::vector<GameObject2D*> vectorGameObject;
	QuadTree* children[4] = {nullptr, nullptr, nullptr, nullptr};
};