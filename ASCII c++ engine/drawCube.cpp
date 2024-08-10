#include "drawCube.h"
#include <cmath>

// Функція для малювання тексту на екрані
void DrawTextOnScreen(HDC hdc, const std::wstring& text, int x, int y) {
    SetBkMode(hdc, TRANSPARENT);  // Робимо фон прозорим
    SetTextColor(hdc, RGB(255, 255, 255));  // Білий колір тексту

    // Малюємо текст на вікні
    TextOutW(hdc, x, y, text.c_str(), static_cast<int>(text.length()));
}

struct Vec3 {
    float x, y, z;
};

// основні точки куба
Vec3 cubeVertices[] = {
    {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f},
    {0.5f, 0.5f, -0.5f},   {-0.5f, 0.5f, -0.5f},
    {-0.5f, -0.5f, 0.5f},  {0.5f, -0.5f, 0.5f},
    {0.5f, 0.5f, 0.5f},    {-0.5f, 0.5f, 0.5f}
};

// сторони куба, які треба малювати
int cubeEdges[][2] = {
    {0, 1}, {1, 2}, {2, 3}, {3, 0},
    {4, 5}, {5, 6}, {6, 7}, {7, 4},
    {0, 4}, {1, 5}, {2, 6}, {3, 7}
};

// функція для обертання куба
void Rotate(Vec3& point, float angleX, float angleY) {
    float cosX = cos(angleX);
    float sinX = sin(angleX);
    float cosY = cos(angleY);
    float sinY = sin(angleY);

    // обертання навколо осі Y
    float x = point.x * cosY - point.z * sinY;
    float z = point.x * sinY + point.z * cosY;
    point.x = x;
    point.z = z;

    // обертання навколо осі X
    float y = point.y * cosX - point.z * sinX;
    point.z = point.y * sinX + point.z * cosX;
    point.y = y;
}

// проекція 3D-точки на 2D-площину
void Project(Vec3 point, int& x, int& y, int windowWidth, int windowHeight) {
    float distance = 3.0f;  // Відстань від камери до об'єкта
    float scale = 500;      // Масштаб

    // Перевірте, що z не дорівнює нулю, щоб уникнути поділу на нуль
    float z = point.z + distance;
    if (z <= 0.0f) z = 0.1f; // Запобігаємо діленню на нуль або негативні значення

    // Проекція точки
    x = static_cast<int>((point.x / z) * scale + windowWidth / 2);
    y = static_cast<int>((point.y / z) * scale + windowHeight / 2);
}

// малювання лінії між двома точками
void DrawLine(HDC hdc, int x1, int y1, int x2, int y2) {
    MoveToEx(hdc, x1, y1, NULL);
    LineTo(hdc, x2, y2);
}

// ункція для зміщення куба
void OffsetCube(Vec3 vertices[], float offsetX, float offsetY, float offsetZ) {
    for (int i = 0; i < 8; i++) {
        vertices[i].x += offsetX;
        vertices[i].y += offsetY;
        vertices[i].z += offsetZ;
    }
}

// малювання куба
void DrawCube(HDC hdc, float angleX, float angleY, int windowWidth, int windowHeight, float cameraX, float cameraY) {
    int projectedX[8], projectedY[8];
    Vec3 rotatedVertices[8];

    // зміщення для трьох кубів
    Vec3 offsets[4] = {
        {-1.0f + cameraX, 0.0f + cameraY, 0.0f},   // Зліва
        {-1.0f + cameraX, -1.0f + cameraY, 0.0f},    // зліва-зверху
        {-1.0f + cameraX, 0.0f + cameraY, -1.0f},     // зліва-спереду
    };

    // малюю кожен куб
    for (int j = 0; j < 4; j++) {
        // Копіюємо вершини куба та зміщуємо їх
        for (int i = 0; i < 8; i++) {
            rotatedVertices[i] = cubeVertices[i];
        }
        OffsetCube(rotatedVertices, offsets[j].x, offsets[j].y, offsets[j].z);

        // Обертаємо та проектуємо всі вершини куба
        for (int i = 0; i < 8; i++) {
            Rotate(rotatedVertices[i], angleX, angleY);
            Project(rotatedVertices[i], projectedX[i], projectedY[i], windowWidth, windowHeight);
        }

        // Малюємо всі грані куба
        for (int i = 0; i < 12; i++) {
            DrawLine(hdc, projectedX[cubeEdges[i][0]], projectedY[cubeEdges[i][0]],
                projectedX[cubeEdges[i][1]], projectedY[cubeEdges[i][1]]);
        }
    }
}
