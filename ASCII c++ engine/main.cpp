#include <windows.h>
#include "window.h"
#include <chrono>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    HWND hwnd = CreateMainWindow(hInstance, nCmdShow);

    if (hwnd == NULL) {
        return 0;
    }

    MSG msg = {};
    auto lastTime = std::chrono::high_resolution_clock::now();
    int frameCount = 0;

    while (true) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                return 0;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Оновлення заголовку з FPS
        frameCount++;
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = currentTime - lastTime;
        if (elapsed.count() >= 1.0f) {
            float fps = frameCount / elapsed.count();
            frameCount = 0;
            lastTime = currentTime;

            wchar_t title[256];
            swprintf_s(title, L"3D Cube - FPS: %.2f", fps);
            SetWindowText(hwnd, title);
        }

        // Перемальовування вікна
        InvalidateRect(hwnd, NULL, FALSE);

        // Затримка для зменшення навантаження на процесор
        Sleep(1);
    }

    return 0;
}
