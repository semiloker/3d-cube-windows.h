#ifndef DRAWCUBE_H
#define DRAWCUBE_H

#include <windows.h>
#include <string>


void DrawCube(HDC hdc, float angleX, float angleY, float cameraX, float cameraY, float cameraZ, int windowWidth, int windowHeight);
void DrawTextOnScreen(HDC hdc, const std::wstring& text, int x, int y);

#endif 