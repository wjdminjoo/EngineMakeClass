#include "Precompiled.h"
#include "QuadTree.h"

void QuadTree::Insert(const Rectangle& rect, GameObject2D* object)
{
	if (object->GetMesh() == nullptr)
		return;

	char sector = InRect(rect);

	if (sector == NULL)
	{
		this->vectorGameObject.push_back(object);
	}
	else
	{
		switch (sector)
		{
		case NW:
			if (children[NW] == nullptr)
			{
				Vector2 min = Vector2(this->rect.Min.X, (this->rect.Min.Y + this->rect.Max.Y) / 2.f);
				Vector2 max = Vector2((this->rect.Min.X + this->rect.Max.X) / 2.f, this->rect.Max.Y);
				children[NW] = new QuadTree(min, max);
			}
			children[NW]->Insert(rect, object);
			return;
		case NE:
			if (children[NE] == nullptr)
			{
				Vector2 min = Vector2((this->rect.Min.X + this->rect.Max.X) / 2.f, (this->rect.Min.Y + this->rect.Max.Y) / 2.f);
				Vector2 max = Vector2(this->rect.Max.X, this->rect.Max.Y);
				children[NE] = new QuadTree(min, max);
			}
			children[NE]->Insert(rect, object);
			return;
		case SW:
			if (children[SW] == nullptr)
			{
				Vector2 min = Vector2(this->rect.Min.X, this->rect.Min.Y);
				Vector2 max = Vector2((this->rect.Min.X + this->rect.Max.X) / 2.f, (this->rect.Min.Y + this->rect.Max.Y) / 2.f);
				children[SW] = new QuadTree(min, max);
			}
			children[SW]->Insert(rect, object);
			return;
		case SE:
			if (children[SE] == nullptr)
			{
				Vector2 min = Vector2((this->rect.Min.X + this->rect.Max.X) / 2.f, this->rect.Min.Y);
				Vector2 max = Vector2(this->rect.Max.X, (this->rect.Min.Y + this->rect.Max.Y) / 2.f);
				children[SE] = new QuadTree(min, max);
			}
			children[SE]->Insert(rect, object);
			return;
		}
	}
}

bool QuadTree::GetSectorObject(const Rectangle& rect, std::vector<GameObject2D*>& object)
{
	bool result = false;

	if (rect.Intersect(this->rect)) //this->rect에 값이 안들어감.
	{
		if (vectorGameObject.size() > 0)
		{
			for (int i = 0; i < vectorGameObject.size(); ++i)
			{
				object.push_back(vectorGameObject[i]);
			}

			for (int i = NW; i < SE + 1; ++i)
			{
				if (children[i] != nullptr)
				{
					children[i]->GetSectorObject(rect, object);
				}
			}

			result = true;
		}
		else
		{
			for (int i = NW; i < SE + 1; ++i)
			{
				if (children[i] != nullptr)
				{
					if (children[i]->GetSectorObject(rect, object))
					{
						result = true;
					}
				}
			}
		}
	}
	return result;
}

void QuadTree::GetAllObject(std::vector<GameObject2D*>& object)
{
	if (vectorGameObject.size() > 0)
	{
		for (int i = 0; i < vectorGameObject.size(); ++i)
		{
			object.push_back(vectorGameObject[i]);
		}

		for (int i = NW; i < SE + 1; ++i)
		{
			if (children[i] != nullptr)
			{
				children[i]->GetAllObject(object);
			}
		}
	}
	else
	{
		for (int i = NW; i < SE + 1; ++i)
		{
			if (children[i] != nullptr)
			{
				children[i]->GetAllObject(object);
			}
		}
	}
}

char QuadTree::InRect(const Rectangle& rect)
{
	char result = NULL;
	
	Vector2 center = Vector2((this->rect.Min.X + this->rect.Max.X) * 0.5f,
		(this->rect.Min.Y + this->rect.Max.Y) * 0.5f); //rect가 nullptr 거부 
	

	if (rect.Min.Y >= center.Y)
	{
		if (rect.Max.X <= center.X)
		{
			result = NW;
		}
		else if (rect.Min.X >= center.X)
		{
			result = NE;
		}

	}
	else if (rect.Max.Y <= center.Y)
	{
		if (rect.Max.X <= center.X)
		{
			result = SW;
		}
		else if (rect.Min.X >= center.X)
		{
			result = SE;
		}
	}
	return result;
}
