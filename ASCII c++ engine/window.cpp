#include "window.h"
#include "drawCube.h"
#include <chrono>

static float angleX = 0.0f;
static float angleY = 0.0f;
static POINT lastMousePos;
static bool firstMouseMovement = true;
static float cameraX = 0.0f;
static float cameraY = 0.0f;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Очищення фону
        HBRUSH hBrush = CreateSolidBrush(RGB(100, 0, 0));
        RECT rect;
        GetClientRect(hwnd, &rect);
        int windowWidth = rect.right - rect.left;
        int windowHeight = rect.bottom - rect.top;

        FillRect(hdc, &rect, hBrush);
        DeleteObject(hBrush);

        // Малювання куба
        DrawCube(hdc, angleX, angleY, windowWidth, windowHeight, cameraX, cameraY);

        // Текст інформації про напрямок камери
        std::wstring directionText = L"Camera Direction: X = " + std::to_wstring(cameraX) + L", Y = " + std::to_wstring(cameraY) +
            L" | Mouse Look: AngleX = " + std::to_wstring(angleX) + L", AngleY = " + std::to_wstring(angleY);
        DrawTextOnScreen(hdc, directionText, 10, 10);

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

        // Визначення зміщення миші
        int dx = mouseX - lastMousePos.x;
        int dy = mouseY - lastMousePos.y;

        // Оновлення кутів обертання
        angleY += dx * 0.01f;
        angleX += dy * 0.01f;

        lastMousePos.x = mouseX;
        lastMousePos.y = mouseY;

        InvalidateRect(hwnd, NULL, FALSE);
        return 0;
    }
    case WM_KEYDOWN: {
        // Рух камери за допомогою клавіш
        switch (wParam) {
        case 'W': cameraY -= 0.1f; break;  // Перемістити камеру вгору
        case 'S': cameraY += 0.1f; break;  // Перемістити камеру вниз
        case 'A': cameraX -= 0.1f; break;  // Перемістити камеру вліво
        case 'D': cameraX += 0.1f; break;  // Перемістити камеру вправо
        }
        InvalidateRect(hwnd, NULL, FALSE);  // Перемалювати вікно
        return 0;
    }
    case WM_LBUTTONDOWN: {
        // Скидання статусу миші
        firstMouseMovement = true;
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
        0,                              // Додаткові стилі вікна
        CLASS_NAME,                     // Ім'я класу
        L"3D Cube",                     // Назва вікна
        WS_OVERLAPPEDWINDOW,            // Стиль вікна
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 500, // Розмір і положення вікна
        NULL,                           // Батьківське вікно
        NULL,                           // Меню
        hInstance,                      // Дескриптор додатка
        NULL                            // Додаткові дані
    );

    if (hwnd != NULL) {
        ShowWindow(hwnd, nCmdShow);
    }

    return hwnd;
}
