#ifndef DRAWCUBE_H
#define DRAWCUBE_H

#include <windows.h>
#include <string>

void DrawCube(HDC hdc, float angleX, float angleY, int windowWidth, int windowHeight, float cameraX, float cameraY);
void DrawTextOnScreen(HDC hdc, const std::wstring& text, int x, int y);

#endif // DRAWCUBE_H
