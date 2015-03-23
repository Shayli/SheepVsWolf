#pragma once

#include <array>
#include <memory>
#include <Graphics/glm.hpp>

typedef enum {
	QuadTree_TopLeft,
	QuadTree_TopRight,
	QuadTree_BottomLeft,
	QuadTree_BottomRight,

	QuadTree_MaxChilds
} QuadTree_Child;

typedef enum {
	QuadTree_Left,
	QuadTree_Right,
	QuadTree_Top,
	QuadTree_Bottom,

	QuadTree_MaxNeighbors
} QuadTree_Neighbor;

typedef enum {
	QuadTree_Leaf,
	QuadTree_Node
} QuadTree_Type;

template <typename T>
class QuadTree {
public:
	T Value;
	std::array<std::shared_ptr<QuadTree>, QuadTree_MaxChilds> Childs;
	std::array<std::shared_ptr<QuadTree>, QuadTree_MaxNeighbors> Neighboors;
	QuadTree* Parent;
	glm::vec2 Position;
	uint32_t Depth, Size;
	int32_t LOD;
	QuadTree_Type Type;

	QuadTree(QuadTree* parent = nullptr) : Parent(parent), Type(QuadTree_Leaf), Depth(0), Size(0), LOD(0)
	{
		for (int i = 0; i < QuadTree_MaxChilds; ++i)
			Childs[i] = nullptr;
		for (int i = 0; i < QuadTree_MaxNeighbors; ++i)
			Neighboors[i] = nullptr;
	}

	void feed(T* data, uint32_t size)
	{
		compose(data, 0, 0, size, size, size);

		reinspect();
	}

	void reinspect()
	{
		fillHoles();
		createNeighborhood();
	}
protected:
	uint32_t compose(T* data, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t size)
	{
		if (x1 == x2 || y1 == y2 || x1 % 2 == 1 || x2 % 2 == 1 || y1 % 2 == 1 || y2 % 2 == 1)
		{
			Position = glm::vec2(x1, y1);
			Type = QuadTree_Leaf;
			Value = data[x1*size + y1];
			Size = x2 - x1;
			//std::cout << Position.x << " " << Position.y << " " << size << std::endl;
			Depth = 1;
			return Depth;
		}
		uint32_t midX = (x2 + x1) / 2;
		uint32_t midY = (y2 + y1) / 2;

		// Top-left corner
		bool needCompose = false;
		T val = data[x1*size + y1];
		for (uint32_t x = x1; x < x2; ++x)
		{
			for (uint32_t y = y1; y < y2; ++y)
			{
				if (val != data[x*size + y])
				{
					//std::cout << "x: " << x << " y: " << y << " " << (int)val << "/" << (int) data[x*size + y] << " " << x1 << " " << y1 << " " << x2 << " " << y2 << " " << size << std::endl;
					needCompose = true;
					break;
				}
			}
			if (needCompose)
				break;
		}

		if (needCompose)
		{

			Childs[QuadTree_TopLeft] = std::make_shared<QuadTree<T>>(this);
			Childs[QuadTree_TopRight] =	std::make_shared<QuadTree<T>>(this);
			Childs[QuadTree_BottomLeft] = std::make_shared<QuadTree<T>>(this);
			Childs[QuadTree_BottomRight] = std::make_shared<QuadTree<T>>(this);

			Childs[QuadTree_TopLeft]->Neighboors[QuadTree_Right] = Childs[QuadTree_TopRight];
			Childs[QuadTree_TopLeft]->Neighboors[QuadTree_Bottom] = Childs[QuadTree_BottomLeft];
			Childs[QuadTree_TopRight]->Neighboors[QuadTree_Left] = Childs[QuadTree_TopLeft];
			Childs[QuadTree_TopRight]->Neighboors[QuadTree_Bottom] = Childs[QuadTree_BottomRight];
			Childs[QuadTree_BottomLeft]->Neighboors[QuadTree_Right] = Childs[QuadTree_BottomRight];
			Childs[QuadTree_BottomLeft]->Neighboors[QuadTree_Top] = Childs[QuadTree_TopLeft];
			Childs[QuadTree_BottomRight]->Neighboors[QuadTree_Left] = Childs[QuadTree_BottomLeft];
			Childs[QuadTree_BottomRight]->Neighboors[QuadTree_Top] = Childs[QuadTree_TopRight];

			Type = QuadTree_Node;
			Depth = std::max(Childs[QuadTree_TopLeft]->compose(data, x1, y1, midX, midY, size), Depth);
			Depth = std::max(Childs[QuadTree_TopRight]->compose(data, midX, y1, x2, midY, size), Depth);
			Depth = std::max(Childs[QuadTree_BottomLeft]->compose(data, x1, midY, midX, y2, size), Depth);
			Depth = std::max(Childs[QuadTree_BottomRight]->compose(data, midX, midY, x2, y2, size), Depth);

			++Depth;
			Position = glm::vec2((x2 + x1) / 2, (y2 + y1) / 2);
			Value = (Childs[QuadTree_TopLeft]->Value + Childs[QuadTree_TopRight]->Value + Childs[QuadTree_BottomLeft]->Value + Childs[QuadTree_BottomRight]->Value) / T(4);
			Size = x2 - x1;
			return Depth;
		}
		else {
			Value = val;
			Type = QuadTree_Leaf;
			Position = glm::vec2((x2 + x1) / 2, (y2 + y1) / 2);
			Depth = 1;
			Size = x2 - x1;
			return Depth;
			//std::cout << Position.x << " " << Position.y << " " << size << std::endl;
		}
	}

	void createNeighborhood()
	{
		if (Type == QuadTree_Leaf)
			return;

		Childs[QuadTree_TopLeft]->Neighboors[QuadTree_Top] = findNeighbor(Childs[QuadTree_TopLeft].get(), QuadTree_Top);
		Childs[QuadTree_TopLeft]->Neighboors[QuadTree_Left] = findNeighbor(Childs[QuadTree_TopLeft].get(), QuadTree_Left);
		Childs[QuadTree_TopRight]->Neighboors[QuadTree_Right] = findNeighbor(Childs[QuadTree_TopRight].get(), QuadTree_Right);
		Childs[QuadTree_TopRight]->Neighboors[QuadTree_Top] = findNeighbor(Childs[QuadTree_TopRight].get(), QuadTree_Top);

		Childs[QuadTree_BottomLeft]->Neighboors[QuadTree_Bottom] = findNeighbor(Childs[QuadTree_BottomLeft].get(), QuadTree_Bottom);
		Childs[QuadTree_BottomLeft]->Neighboors[QuadTree_Left] = findNeighbor(Childs[QuadTree_BottomLeft].get(), QuadTree_Left);
		Childs[QuadTree_BottomRight]->Neighboors[QuadTree_Right] = findNeighbor(Childs[QuadTree_BottomRight].get(), QuadTree_Right);
		Childs[QuadTree_BottomRight]->Neighboors[QuadTree_Bottom] = findNeighbor(Childs[QuadTree_BottomRight].get(), QuadTree_Bottom);

		Childs[QuadTree_TopLeft]->createNeighborhood();
		Childs[QuadTree_TopRight]->createNeighborhood();
		Childs[QuadTree_BottomLeft]->createNeighborhood();
		Childs[QuadTree_BottomRight]->createNeighborhood();
	}

	std::shared_ptr<QuadTree<T>>& findNeighbor(QuadTree* src, QuadTree_Neighbor neighbor)
	{
		if (src->Parent == nullptr)
			return NullNode;
		if (src->Parent->Childs[QuadTree_TopLeft].get() == src)
		{
			if (neighbor == QuadTree_Left || neighbor == QuadTree_Top)
			{
				auto& tmp = findNeighbor(src->Parent, neighbor);
				if (!tmp)
					return NullNode;
				if (tmp->Type == QuadTree_Leaf || tmp->Size <= src->Size)
					return tmp;
				return (neighbor == QuadTree_Left) ? tmp->Childs[QuadTree_TopRight] : tmp->Childs[QuadTree_BottomLeft];
			}
			else
				return src->Neighboors[neighbor];
		}
		else if (src->Parent->Childs[QuadTree_TopRight].get() == src)
		{
			if (neighbor == QuadTree_Right || neighbor == QuadTree_Top)
			{
				auto& tmp = findNeighbor(src->Parent, neighbor);
				if (!tmp)
					return NullNode;
				if (tmp->Type == QuadTree_Leaf || tmp->Size <= src->Size)
					return tmp;
				return (neighbor == QuadTree_Right) ? tmp->Childs[QuadTree_TopLeft] : tmp->Childs[QuadTree_BottomRight];
			}
			else 
				return src->Neighboors[neighbor];
		}
		else if (src->Parent->Childs[QuadTree_BottomLeft].get() == src)
		{
			if (neighbor == QuadTree_Left || neighbor == QuadTree_Bottom)
			{
				auto& tmp = findNeighbor(src->Parent, neighbor);
				if (!tmp)
					return NullNode;
				if (tmp->Type == QuadTree_Leaf || tmp->Size <= src->Size)
					return tmp;
				else
				{
					return (neighbor == QuadTree_Left) ? tmp->Childs[QuadTree_BottomRight] : tmp->Childs[QuadTree_TopLeft];
				}
			}
			else
				return src->Neighboors[neighbor];
		}

		else
		{
			if (neighbor == QuadTree_Right || neighbor == QuadTree_Bottom)
			{
				auto& tmp = findNeighbor(src->Parent, neighbor);
				if (!tmp)
					return NullNode;
				if (tmp->Type == QuadTree_Leaf || tmp->Size <= src->Size)
					return tmp;
				return (neighbor == QuadTree_Right) ? tmp->Childs[QuadTree_BottomLeft] : tmp->Childs[QuadTree_TopRight];
			}
			else
				return src->Neighboors[neighbor];
		}
	}

	void fillHoles()
	{
		if (!Type == QuadTree_Leaf)
		{
			for (auto& c : Childs)
				c->fillHoles();
		}
		else
		{
			auto currNeighbor = findNeighbor(this, QuadTree_Top);
			bool needCreate = false;
			if (currNeighbor && !currNeighbor->isLeaf() &&
				(currNeighbor->Childs[QuadTree_BottomLeft]->Depth != 1 || currNeighbor->Childs[QuadTree_BottomRight]->Depth != 1))
				needCreate = true;

			currNeighbor = findNeighbor(this, QuadTree_Bottom);
			if (currNeighbor && !currNeighbor->isLeaf() &&
				(currNeighbor->Childs[QuadTree_TopLeft]->Depth != 1 || currNeighbor->Childs[QuadTree_TopRight]->Depth != 1))
				needCreate = true;


			currNeighbor = findNeighbor(this, QuadTree_Left);
			if (currNeighbor && !currNeighbor->isLeaf() &&
				(currNeighbor->Childs[QuadTree_TopRight]->Depth != 1 || currNeighbor->Childs[QuadTree_BottomRight]->Depth != 1))
				needCreate = true;

			currNeighbor = findNeighbor(this, QuadTree_Right);
			if (currNeighbor && !currNeighbor->isLeaf() &&
				(currNeighbor->Childs[QuadTree_TopLeft]->Depth != 1 || currNeighbor->Childs[QuadTree_BottomLeft]->Depth != 1))
				needCreate = true;

			if (needCreate)
			{
				Type = QuadTree_Node;
				Depth++;
				if (!Childs[QuadTree_TopLeft])
				{
					auto child = std::make_shared<QuadTree<T>>(this);
					child->Depth = 1;
					child->Size = Size / 2;
					child->Type = QuadTree_Leaf;
					child->Position = Position + glm::vec2(-(int32_t) Size / 4, -(int32_t) Size / 4);
					child->Value = Value;
					Childs[QuadTree_TopLeft] = child;
				}

				if (!Childs[QuadTree_TopRight])
				{
					auto child = std::make_shared<QuadTree<T>>(this);
					child->Depth = 1;
					child->Size = Size / 2;
					child->Type = QuadTree_Leaf;
					child->Position = Position + glm::vec2(Size / 4, -(int32_t) Size / 4);
					child->Value = Value;
					Childs[QuadTree_TopRight] = child;
				}

				if (!Childs[QuadTree_BottomLeft])
				{
					auto child = std::make_shared<QuadTree<T>>(this);
					child->Depth = 1;
					child->Size = Size / 2;
					child->Type = QuadTree_Leaf;
					child->Position = Position + glm::vec2(-(int32_t) Size / 4, Size / 4);
					child->Value = Value;
					Childs[QuadTree_BottomLeft] = child;
				}
				if (!Childs[QuadTree_BottomRight])
				{
					auto child = std::make_shared<QuadTree<T>>(this);
					child->Depth = 1;
					child->Size = Size / 2;
					child->Type = QuadTree_Leaf;
					child->Position = Position + glm::vec2(Size / 4, Size / 4);
					child->Value = Value;
					Childs[QuadTree_BottomRight] = child;
				}

				Childs[QuadTree_TopLeft]->Neighboors[QuadTree_Right] = Childs[QuadTree_TopRight];
				Childs[QuadTree_TopLeft]->Neighboors[QuadTree_Bottom] = Childs[QuadTree_BottomLeft];
				Childs[QuadTree_TopRight]->Neighboors[QuadTree_Left] = Childs[QuadTree_TopLeft];
				Childs[QuadTree_TopRight]->Neighboors[QuadTree_Bottom] = Childs[QuadTree_BottomRight];
				Childs[QuadTree_BottomLeft]->Neighboors[QuadTree_Right] = Childs[QuadTree_BottomRight];
				Childs[QuadTree_BottomLeft]->Neighboors[QuadTree_Top] = Childs[QuadTree_TopLeft];
				Childs[QuadTree_BottomRight]->Neighboors[QuadTree_Left] = Childs[QuadTree_BottomLeft];
				Childs[QuadTree_BottomRight]->Neighboors[QuadTree_Top] = Childs[QuadTree_TopRight];

				for (auto& c : Childs)
					c->fillHoles();
			}
		}
	}

	inline bool isLeaf()
	{
		return Type == QuadTree_Leaf;
	}

	static std::shared_ptr<QuadTree> NullNode;
};


template <typename T>
std::shared_ptr<QuadTree<T>> QuadTree<T>::NullNode = nullptr;