#include "drawCube.h"
#include <cmath>

struct Vec3 {
    float x, y, z;
};

// ������ ����� ����
Vec3 cubeVertices[] = {
    {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f},
    {0.5f, 0.5f, -0.5f},   {-0.5f, 0.5f, -0.5f},
    {-0.5f, -0.5f, 0.5f},  {0.5f, -0.5f, 0.5f},
    {0.5f, 0.5f, 0.5f},    {-0.5f, 0.5f, 0.5f}
};

// ������� ����, �� ����� ��������
int cubeEdges[][2] = {
    {0, 1}, {1, 2}, {2, 3}, {3, 0},
    {4, 5}, {5, 6}, {6, 7}, {7, 4},
    {0, 4}, {1, 5}, {2, 6}, {3, 7}
};

// ������� ��� ��������� ����
void Rotate(Vec3& point, float angleX, float angleY) {
    float cosX = cos(angleX);
    float sinX = sin(angleX);
    float cosY = cos(angleY);
    float sinY = sin(angleY);

    // ��������� ������� �� Y
    float x = point.x * cosY - point.z * sinY;
    float z = point.x * sinY + point.z * cosY;
    point.x = x;
    point.z = z;

    // ��������� ������� �� X
    float y = point.y * cosX - point.z * sinX;
    point.z = point.y * sinX + point.z * cosX;
    point.y = y;
}

// �������� 3D-����� �� 2D-�������
void Project(Vec3 point, int& x, int& y) {
    float distance = 2.0f;
    x = static_cast<int>((point.x / (point.z + distance)) * 800 / 2 + 800 / 2);
    y = static_cast<int>((point.y / (point.z + distance)) * 600 / 2 + 600 / 2);
}

// ��������� �� �� ����� �������
void DrawLine(HDC hdc, int x1, int y1, int x2, int y2) {
    MoveToEx(hdc, x1, y1, NULL);
    LineTo(hdc, x2, y2);
}

// ��������� ����
void DrawCube(HDC hdc, float angleX, float angleY) {
    int projectedX[8], projectedY[8];
    Vec3 rotatedVertices[8];

    // �������� �� ��������� �� ������� ����
    for (int i = 0; i < 8; i++) {
        rotatedVertices[i] = cubeVertices[i];
        Rotate(rotatedVertices[i], angleX, angleY);
        Project(rotatedVertices[i], projectedX[i], projectedY[i]);
    }

    // ������� �� ���� ����
    for (int i = 0; i < 12; i++) {
        DrawLine(hdc, projectedX[cubeEdges[i][0]], projectedY[cubeEdges[i][0]],
            projectedX[cubeEdges[i][1]], projectedY[cubeEdges[i][1]]);
    }
}
