#include "pch.h"
#include "BSTRender.h"

void BSTRender::Update()
{

}

void BSTRender::Render(HDC hdc, int width, int height)
{
	DrawBST(hdc, &bst, width, height);
}

void BSTRender::Init(double node_scale)
{
	NODE_SCALE = node_scale;
}

void BSTRender::SetNodeScale(double node_scale)
{
	NODE_SCALE = node_scale;
}

void BSTRender::insert(int num)
{
	bst.insert(make_myPair(num, 0));
}

void BSTRender::DrawLine(HDC hdc, Vec2 StartPos, Vec2 EndPos)
{
	MoveToEx(hdc, (int)StartPos.x, (int)StartPos.y, NULL);
	LineTo(hdc, (int)EndPos.x, (int)EndPos.y);
}
