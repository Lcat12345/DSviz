#pragma once

#include "BST.h"

class BSTRender
{
public:
	void Update();
	void Render(HDC hdc, int width, int height);
	void Init(double node_scale);
	void SetNodeScale(double node_scale);

	void insert(int num);

private:
	template<typename Key, typename Value>
	void DrawBST(HDC hdc, BST<Key, Value>* tree, int width, int height);
	template<typename Key, typename Value>
	int GetTreeHeight(BST<Key, Value>* tree);
	template<typename Key, typename Value>
	int GetTreeHeightFromNode(Node<Key, Value>* node);
	template<typename Key, typename Value>
	int GetTreeWidthFromNode(Node<Key, Value>* node);
	template<typename Key, typename Value>
	void DrawNodes(HDC hdc, Node<Key, Value>* node, Vec2 pos);
	template<typename Key, typename Value>
	void DrawNode(HDC hdc, Node<Key, Value>* node, Vec2 pos);
	template<typename Key, typename Value>
	void DrawLines(HDC hdc, Node<Key, Value>* node, Vec2 pos);
	void DrawLine(HDC hdc, Vec2 StartPos, Vec2 EndPos);


private:
	BST<int, int> bst;
	
public:
	double NODE_SCALE;			// radius
};

template<typename Key, typename Value>
inline void BSTRender::DrawBST(HDC hdc, BST<Key, Value>* tree, int width, int height)
{
	Vec2 root_position;
	root_position.x = (double)width / 2.0;
	root_position.y = NODE_SCALE + 20.0;

	DrawLines(hdc, tree->GetRoot(), root_position);
	DrawNodes(hdc, tree->GetRoot(), root_position);
}

template<typename Key, typename Value>
inline int BSTRender::GetTreeHeight(BST<Key, Value>* tree)
{
	if (nullptr == tree->GetRoot())
		return 0;
	return GetTreeHeightFromNode(tree->GetRoot());
}

template<typename Key, typename Value>
inline int BSTRender::GetTreeHeightFromNode(Node<Key, Value>* node)
{
	if (nullptr == node)
		return 0;

	int LeftHeight = 1;
	int RightHeight = 1;

	if (nullptr != node->LeftChild)
		LeftHeight = GetTreeHeightFromNode(node->LeftChild) + 1;

	if (nullptr != node->RightChild)
		RightHeight = GetTreeHeightFromNode(node->RightChild) + 1;

	return (LeftHeight > RightHeight ? LeftHeight : RightHeight);
}

template<typename Key, typename Value>
inline int BSTRender::GetTreeWidthFromNode(Node<Key, Value>* node)
{
	if(node == nullptr)
		return 0;

	int left = 0;
	int right = 0;

	// 자식이 없다.
	if (nullptr == node->LeftChild && nullptr == node->RightChild)
		return 1;
	// 왼쪽 자식만 있다.
	else if (node->LeftChild && nullptr == node->RightChild)
		left = 1 + GetTreeWidthFromNode(node->LeftChild);
	// 오른쪽 자식만 있다.
	else if (node->RightChild && nullptr == node->LeftChild)
		right = 1 + GetTreeWidthFromNode(node->RightChild);
	// 자식이 모두 있다.
	else
	{
		left = GetTreeWidthFromNode(node->LeftChild);
		right = GetTreeWidthFromNode(node->RightChild);
	}

	return left + right;
}

template<typename Key, typename Value>
inline void BSTRender::DrawNodes(HDC hdc, Node<Key, Value>* node, Vec2 pos)
{
	Vec2 blank;
	int distance;

	blank.x = NODE_SCALE * 1.25;
	blank.y = NODE_SCALE * 2.5;

	if (nullptr != node)
	{
		if (nullptr == node->parent)	// node가 root이니?
		{
			DrawNode(hdc, node, pos);
		}
		else if (bst.isRight(node)) // node가 오른쪽 자식이니?
		{
			distance = (GetTreeWidthFromNode(node->LeftChild) + 1) * blank.x;

			pos.x += distance;
			pos.y += blank.y;
			DrawNode(hdc, node, pos);
		}
		else if (bst.isLeft(node)) // node가 왼쪽 자식이니?
		{
			distance = (GetTreeWidthFromNode(node->RightChild) + 1) * blank.x;
						
			pos.x -= distance;
			pos.y += blank.y;
			DrawNode(hdc, node, pos);
		}

		DrawNodes(hdc, node->LeftChild, pos);
		DrawNodes(hdc, node->RightChild, pos);
	}
}

template<typename Key, typename Value>
inline void BSTRender::DrawNode(HDC hdc, Node<Key, Value>* node, Vec2 pos)
{
	Ellipse(hdc
		, pos.x - NODE_SCALE
		, pos.y - NODE_SCALE
		, pos.x + NODE_SCALE
		, pos.y + NODE_SCALE
	);

	char str[255] = {};
	sprintf_s(str, "%d", node->pair.first);
	RECT textRect = { 0, 0, 0, 0 }; // right = width, bottom = height
	DrawTextA(hdc, str, -1, &textRect, DT_CALCRECT);

	SetTextAlign(hdc, TA_CENTER);	// 텍스트의 중앙을 postion으로
	double padding = textRect.bottom / 2.0;	// 텍스트 높이의 절반 만큼 올리기
	TextOutA(hdc, (int)pos.x, (int)(pos.y - padding), str, (int)strlen(str));
		
	SetTextAlign(hdc, TA_TOP);	// 원상복구

	/*std::wstring wstr = std::to_wstring(node->pair.first);
	TextOut(hdc, pos.x, pos.y, wstr.c_str(), lstrlen(wstr.c_str()));*/
}

template<typename Key, typename Value>
inline void BSTRender::DrawLines(HDC hdc, Node<Key, Value>* node, Vec2 pos)
{
	Vec2 blank;
	int distance;

	blank.x = NODE_SCALE * 1.25;
	blank.y = NODE_SCALE * 2.5;

	if (nullptr != node)
	{
		if (nullptr == node->parent)	// node가 root이니?
		{
			
		}
		else if (bst.isRight(node)) // node가 오른쪽 자식이니?
		{
			distance = (GetTreeWidthFromNode(node->LeftChild) + 1) * blank.x;

			Vec2 prevPos = pos;
			pos.x += distance;
			pos.y += blank.y;
			DrawLine(hdc, prevPos, pos);
		}
		else if (bst.isLeft(node)) // node가 왼쪽 자식이니?
		{
			distance = (GetTreeWidthFromNode(node->RightChild) + 1) * blank.x;

			Vec2 prevPos = pos;
			pos.x -= distance;
			pos.y += blank.y;
			DrawLine(hdc, prevPos, pos);
		}

		DrawLines(hdc, node->LeftChild, pos);
		DrawLines(hdc, node->RightChild, pos);
	}
}
