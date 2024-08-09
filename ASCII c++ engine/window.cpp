#include "window.h"
#include "drawCube.h"
#include <chrono>

static float angleX = 0.0f;
static float angleY = 0.0f;
static POINT lastMousePos;
static bool firstMouseMovement = true;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // ������� ���
        HBRUSH hBrush = CreateSolidBrush(RGB(110, 0, 0));
        RECT rect;
        GetClientRect(hwnd, &rect);
        FillRect(hdc, &rect, hBrush);
        DeleteObject(hBrush);

        // ������� ���
        DrawCube(hdc, angleX, angleY);

        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_MOUSEMOVE: {
        int mouseX = LOWORD(lParam);
        int mouseY = HIWORD(lParam);

        if (firstMouseMovement) {
            lastMousePos.x = mouseX;
            lastMousePos.y = mouseY;
            firstMouseMovement = false;
        }

        // ���������� ������� ����
        int dx = mouseX - lastMousePos.x;
        int dy = mouseY - lastMousePos.y;

        // ��������� ���� ���������
        angleY += dx * 0.01f;
        angleX += dy * 0.01f;

        lastMousePos.x = mouseX;
        lastMousePos.y = mouseY;

        InvalidateRect(hwnd, NULL, FALSE);
        return 0;
    }
    case WM_DESTROY: {
        PostQuitMessage(0);
        return 0;
    }
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
}

HWND CreateMainWindow(HINSTANCE hInstance, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"3D Cube Window Class";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,                              // �������� ���� ����
        CLASS_NAME,                     // ��'� �����
        L"3D Cube",                     // ����� ����
        WS_OVERLAPPEDWINDOW,            // ����� ����
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, // ����� � ��������� ����
        NULL,                           // ���������� ����
        NULL,                           // ����
        hInstance,                      // ���������� �������
        NULL                            // �������� ���
    );

    if (hwnd != NULL) {
        ShowWindow(hwnd, nCmdShow);
    }

    return hwnd;
}
