#include "window.h"
#include "drawCube.h"

static float angleX = 0.0f;  // ��� ��������� �� X
static float angleY = 0.0f;  // ��� ��������� �� Y
static POINT lastMousePos;
static bool firstMouseMovement = true;
static bool cursorLocked = false;  // ���� �������
static float cameraX = 0.0f;  // ��������� ������ �� X
static float cameraY = 0.0f;  // ��������� ������ �� Y
static float cameraZ = 0.0f;  // ��������� ������ �� Z

// ̳��������� ��� ��������� ���� X
const float MIN_ANGLE_X = 49.0f;
const float MAX_ANGLE_X = 51.0f;

// ������� ��� ���������� ������� � ����� ����
void LockCursor(HWND hwnd) {
    RECT rect;
    GetClientRect(hwnd, &rect);
    MapWindowPoints(hwnd, NULL, (LPPOINT)&rect, 2);
    ClipCursor(&rect);
    ShowCursor(FALSE);  // ��������� ������
    cursorLocked = true;
}

// ������� ��� ��������� �������
void UnlockCursor() {
    ClipCursor(NULL);
    ShowCursor(TRUE);  // �������� ������
    cursorLocked = false;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // �������� ����
        HBRUSH hBrush = CreateSolidBrush(RGB(100, 0, 0));
        RECT rect;
        GetClientRect(hwnd, &rect);
        int windowWidth = rect.right - rect.left;
        int windowHeight = rect.bottom - rect.top;

        FillRect(hdc, &rect, hBrush);
        DeleteObject(hBrush);

        // ��������� ����
        DrawCube(hdc, angleX, angleY, cameraX, cameraY, cameraZ, windowWidth, windowHeight);

        // ����� ���������� ��� �������� ������
        std::wstring directionText = L"Camera Position: X = " + std::to_wstring(cameraX) + L", Y = " + std::to_wstring(cameraY) +
            L", Z = " + std::to_wstring(cameraZ) +
            L" | Mouse Look: AngleX = " + std::to_wstring(angleX) + L", AngleY = " + std::to_wstring(angleY);
        DrawTextOnScreen(hdc, directionText, 10, 10);

        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_MOUSEMOVE: {
        if (cursorLocked) {
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

            // ��������� ���� X
            if (angleX < MIN_ANGLE_X) angleX = MIN_ANGLE_X;
            if (angleX > MAX_ANGLE_X) angleX = MAX_ANGLE_X;

            lastMousePos.x = mouseX;
            lastMousePos.y = mouseY;

            InvalidateRect(hwnd, NULL, FALSE);
        }
        return 0;
    }
    case WM_KEYDOWN: {
        switch (wParam) {
        case VK_ESCAPE: {
            // ��������� ������
            if (cursorLocked) {
                UnlockCursor();
            }
            else {
                LockCursor(hwnd);
            }
            break;
        }
        case 'W': cameraX -= sin(angleY) * 0.1f; cameraZ -= cos(angleY) * 0.1f; break;  // ��� ������
        case 'S': cameraX += sin(angleY) * 0.1f; cameraZ += cos(angleY) * 0.1f; break;  // ��� �����
        case 'A': cameraX += cos(angleY) * 0.1f; cameraZ -= sin(angleY) * 0.1f; break;  // ��� ����
        case 'D': cameraX -= cos(angleY) * 0.1f; cameraZ += sin(angleY) * 0.1f; break;  // ��� ������
        case VK_UP:
            cameraY -= 0.1f;
            if (cameraY < MIN_ANGLE_X) cameraY = MIN_ANGLE_X;  // ��������� �� Y
            break;  // ��� �����
        case VK_DOWN:
            cameraY += 0.1f;
            if (cameraY > MAX_ANGLE_X) cameraY = MAX_ANGLE_X;  // ��������� �� Y
            break;  // ��� ����
        }
        InvalidateRect(hwnd, NULL, FALSE);  // ������������ ����
        return 0;
    }
    case WM_LBUTTONDOWN: {
        // �������� ������� ����
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
        LockCursor(hwnd);  // �������� ������, ���� ���� ��������
    }

    return hwnd;
}
