#pragma once
#include <Windows.h>
#include <math.h>
#define RAD(x) 3.14 / 180 * (x)

void DrawBoard(HDC hdc, RECT bSize, int row, int column)
{
	int width = (bSize.right - bSize.left) / column;
	int height = (bSize.bottom - bSize.top) / row;

	for (int r = 0; r <= row; r++)	// 가로선
	{
		MoveToEx(hdc, bSize.left, bSize.top + (height * r), NULL);
		LineTo(hdc, bSize.right, bSize.top + (height * r));
	}
	for (int c = 0; c <= column; c++)	// 세로선
	{
		MoveToEx(hdc, bSize.left + (width * c), bSize.top, NULL);
		LineTo(hdc, bSize.left + (width * c), bSize.bottom);
	}
}
void DrawBoard(HDC hdc, int sX, int sY, int width, int height, int row, int column)
{
	for (int r = 0; r <= row; r++)
	{
		MoveToEx(hdc, sX, sY + (height * r), NULL);
		LineTo(hdc, sX + (width * column), sY + (height * r));
	}
	for (int c = 0; c <= column; c++)
	{
		MoveToEx(hdc, sX + (width * c), sY, NULL);
		LineTo(hdc, sX + (width * c), sY + (height * row));
	}
}

double GetDirection(POINT pos1, POINT pos2)
{
	int xDiff = pos1.x - pos2.x;
	int yDiff = pos1.y - pos2.y;

	return sqrt((double)((xDiff * xDiff) + (yDiff * yDiff)));
}
bool IsCollision(POINT cs1, int cr1, POINT cs2, int cr2)
{
	double dir = GetDirection(cs1, cs2);

	if (dir <= cr1 + cr2) return TRUE;
	else return FALSE;
}
double GetDirection(RECT pos1, RECT pos2)
{
	int xDiff = pos1.right - pos2.left;
	int yDiff = pos1.bottom - pos2.top;

	return sqrt((double)((xDiff * xDiff) + (yDiff * yDiff)));
}
bool IsCollision(RECT cs1, int cr1, RECT cs2, int cr2)
{
	double dir = GetDirection(cs1, cs2);

	if (dir <= cr1 + cr2) return TRUE;
	else return FALSE;
}
int IsCollision(RECT r1, RECT r2)
{
	// is r1 collision with r2 ?
	enum DIR { NO, LEFT, TOP, RIGHT, BOTTOM };

	int mx = (r1.right - r1.left) / 2 + r1.left;
	int my = (r1.bottom - r1.top) / 2 + r1.top;
	int xCollision = NO;
	int yCollision = NO;

	if (((r1.left <= r2.left && r2.left <= r1.right) && (r2.top <= r1.top && r1.top <= r2.bottom)) ||
		((r1.left <= r2.left && r2.left <= r1.right) && (r2.top <= r1.bottom && r1.bottom <= r2.bottom)))
		xCollision = LEFT;
	else if (((r1.left <= r2.right && r2.right <= r1.right) && (r2.top <= r1.top && r1.top <= r2.bottom)) ||
		((r1.left <= r2.right && r2.right <= r1.right) && (r2.top <= r1.bottom && r1.bottom <= r2.bottom)))
		xCollision = RIGHT;
	if (((r1.top <= r2.top && r2.top <= r1.bottom) && (r2.left <= r1.left && r1.left <= r2.right)) ||
		((r1.top <= r2.top && r2.top <= r1.bottom) && (r2.left <= r1.right && r1.right <= r2.right)))
		yCollision = TOP;
	else if (((r1.top <= r2.bottom && r2.bottom <= r1.bottom) && (r2.left <= r1.left && r1.left <= r2.right)) ||
		((r1.top <= r2.bottom && r2.bottom <= r1.bottom) && (r2.left <= r1.right && r1.right <= r2.right)))
		yCollision = BOTTOM;

	if (xCollision != NO && yCollision != NO)
	{
		int xDiff = 0, yDiff = 0;

		if (xCollision == LEFT)
			xDiff = r2.left - r1.right;
		else if (xCollision == RIGHT)
			xDiff = r1.left - r2.right;
		if (yCollision == TOP)
			yDiff = r2.top - r1.bottom;
		else if (yCollision == BOTTOM)
			yDiff = r1.top - r2.bottom;

		if (xDiff < 0) xDiff *= (-1);
		if (yDiff < 0) yDiff *= (-1);

		return (xDiff < yDiff ? xCollision : yCollision);
	}
	else
		return (xCollision > yCollision ? xCollision : yCollision);
}