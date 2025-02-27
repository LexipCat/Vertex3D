#include "v3deCore.h"

void RemoveCursor()
{
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curInfo.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

Vector2 GetCurrentCursorPos()
{
    Vector2 curPoint;
    CONSOLE_SCREEN_BUFFER_INFO curInfo;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curPoint.x = curInfo.dwCursorPosition.X;
    curPoint.y = curInfo.dwCursorPosition.Y;
    return curPoint;
}

void SetCurrentCursorPos(Vector2 mousePos)
{
    COORD pos = { mousePos.x,mousePos.y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void resetTransform(Transform* transform) {
    transform->position = { 0,0,0 };
    transform->rotation = { 0,0,0 };
}

void drawLine(char** screen, int screenWidth, int screenHeight, Vector2 p1, Vector2 p2) {
    int x1 = p1.x;
    int y1 = p1.y;
    int x2 = p2.x;
    int y2 = p2.y;

    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2, e2;

    while (1) {
        // 화면 범위 안에 있는 경우에만 그리기
        if (x1 >= 0 && x1 < screenWidth && y1 >= 0 && y1 < screenHeight) {
            screen[y1][x1] = '.';
        }
        if (x1 == x2 && y1 == y2) break;
        e2 = err;
        if (e2 > -dx) { err -= dy; x1 += sx; }
        if (e2 < dy) { err += dx; y1 += sy; }
    }
}

Vector3 rotateVector(Vector3 center, Vector3 target, Vector3 rotation) {
    Vector3 result = {
        target.x - center.x,
        target.y - center.y,
        target.z - center.z
    };

    float radX = rotation.x * PI / 180.0f;
    float radY = rotation.y * PI / 180.0f;
    float radZ = rotation.z * PI / 180.0f;

    float x2 = cos(radY) * result.x + sin(radY) * result.z;
    float z2 = -sin(radY) * result.x + cos(radY) * result.z;
    result.x = x2;
    result.z = z2;

    float y1 = cos(radX) * result.y - sin(radX) * result.z;
    float z1 = sin(radX) * result.y + cos(radX) * result.z;
    result.y = y1;
    result.z = z1;

    float x3 = cos(radZ) * result.x - sin(radZ) * result.y;
    float y3 = sin(radZ) * result.x + cos(radZ) * result.y;
    result.x = x3;
    result.y = y3;

    return result;
}

Vector3 lookAt(Vector3 pos, Vector3 target) {
    Vector3 direction = {
        target.x - pos.x,
        target.y - pos.y,
        target.z - pos.z
    };

    Vector3 normalizedDirection = normalize(direction);

    float pitch = atan2(normalizedDirection.y, sqrt(normalizedDirection.x * normalizedDirection.x + normalizedDirection.z * normalizedDirection.z)) * (180 / PI);
    float yaw = -atan2(normalizedDirection.x, normalizedDirection.z) * (180 / PI);

    float roll = 0;

    Vector3 finalRot = { pitch, yaw, roll };
    return finalRot;
}
Vector2* worldVertex_Convert(int screenWidth, int screenHeight, Object obj, Camera camera) {
    Vector2* sp = (Vector2*)malloc(sizeof(Vector2) * obj.v_Length);

    for (int i = 0; i < obj.v_Length; i++) {
        Vector3 fixedVertexPos = {
            obj.vertices[i].x + obj.transform.position.x * 2,
            obj.vertices[i].y + obj.transform.position.y * 2,
            obj.vertices[i].z + obj.transform.position.z * 2
        };

        Vector3 rotatedVertex = rotateVector(obj.transform.position, fixedVertexPos, obj.transform.rotation);

        Vector3 rotatedCamera = rotateVector(camera.transform.position, rotatedVertex, camera.transform.rotation);

        Vector3 worldVertex = rotatedCamera;

        if (worldVertex.z > 0) {
            float Zscale = tan(camera.fov * 0.5f * PI / 180.0f) * worldVertex.z;
            sp[i].x = (int)((worldVertex.x / Zscale + 1) * 0.5f * screenWidth);
            sp[i].y = (int)((1 - (worldVertex.y / Zscale + 1) * 0.5f) * screenHeight);
        }
        else {
            sp[i].x = -1; // 카메라 뒤에 있는 꼭짓점은 무시
            sp[i].y = -1;
        }
    }

    return sp;
}

void screenWrite(char** screen, Vector2 pos, const char* str, int varsize, ...) {
    va_list varlist;
    va_start(varlist, varsize);

    int Overflow = 0;

    for (int i = 0; i < strlen(str); i++) {
        screen[pos.y][pos.x + i + Overflow] = str[i];

        if (str[i] == '%') {
            int argVar = va_arg(varlist, int);

            if (argVar < 0) {
                argVar *= -1;
                screen[pos.y][pos.x + i + Overflow] = '-';
                Overflow += 1;
            }

            int temp = argVar, count = 0;
            while (temp > 0) {
                temp /= 10;
                ++count;
            }

            if (!count) {
                screen[pos.y][pos.x + i + Overflow] = '0';
            }
            else {
                temp = argVar;
                for (int j = count - 1; j >= 0; j--) {
                    screen[pos.y][pos.x + i + Overflow + j] = '0' + (temp % 10);
                    temp /= 10;
                }
                Overflow += count - 1;
            }
        }
    }

    va_end(varlist);
}

void printScreen(char** screen, int screenWidth, int screenHeight, Vector2 RenderPos) {
    SetCurrentCursorPos(RenderPos);

    for (int y = 0; y < screenHeight; y++) {
        for (int x = 0; x < screenWidth; x++) {
            printf("%c", screen[y][x]);
        }
        SetCurrentCursorPos({ RenderPos.x,RenderPos.y + y });
    }
}

char** screenInit(int screenWidth, int screenHeight) {
    char** screen = (char**)malloc(screenHeight * sizeof(char*));
    for (int i = 0; i < screenHeight; i++) {
        screen[i] = (char*)malloc(screenWidth * sizeof(char));
        for (int j = 0; j < screenWidth; j++) {
            screen[i][j] = ' ';
        }
    }
    return screen;
}

void Render(char** screen, int screenWidth, int screenHeight, Vector2 drawOffset, Camera camera, int GizmoType, int objArgs, ...) {
    va_list objlist;
    va_start(objlist, objArgs);

    for (int i = 0; i < objArgs; i++) {
        Object argObj = va_arg(objlist, Object);
        Vector2* sp = worldVertex_Convert(screenWidth, screenHeight, argObj, camera);

        for (int i = 0; i < argObj.v_Length; i++) {
            sp[i].x += drawOffset.x;
            sp[i].y += drawOffset.y;
        }

#pragma region DrawLine
        for (int i = 0; i < argObj.l_Length; i++) {
            int x1 = sp[argObj.vertexLine[i].a].x;
            int y1 = sp[argObj.vertexLine[i].a].y;
            int x2 = sp[argObj.vertexLine[i].b].x;
            int y2 = sp[argObj.vertexLine[i].b].y;

            int dx = abs(x2 - x1);
            int dy = abs(y2 - y1);
            int sx = x1 < x2 ? 1 : -1;
            int sy = y1 < y2 ? 1 : -1;
            int err = (dx > dy ? dx : -dy) / 2;

            while (1) {
                if (x1 >= 0 + drawOffset.x && x1 < screenWidth + drawOffset.x && y1 >= 0 + drawOffset.y && y1 < screenHeight + drawOffset.y) {
                    screen[y1][x1] = '.';
                }
                if (x1 == x2 && y1 == y2) break;
                int e2 = err;
                if (e2 > -dx) {
                    err -= dy;
                    x1 += sx;
                }
                if (e2 < dy) {
                    err += dx;
                    y1 += sy;
                }
            }
        }
#pragma endregion

#pragma region DrawVertex
        for (int i = 0; i < argObj.v_Length; i++) {
            if (sp[i].x >= 0 + drawOffset.x && sp[i].x < screenWidth + drawOffset.x && sp[i].y >= 0 + drawOffset.y && sp[i].y < screenHeight + drawOffset.y) {
                if (argObj.isCustomChar)
                    screen[(int)sp[i].y][(int)sp[i].x] = argObj.verteicesChar[i];
                else {
                    switch (GizmoType)
                    {
                    case 0:
                        screenWrite(screen, { sp[i].x,sp[i].y }, "#", 0);
                        break;
                    case 1:
                        screenWrite(screen, { sp[i].x,sp[i].y }, "%", 1, i);
                        break;
                    case 2:
                        if (sp[i].x >= 5 && sp[i].x < screenWidth - 5 && sp[i].y >= 0 && sp[i].y < screenHeight - 1) {
                            screenWrite(screen, { sp[i].x,sp[i].y }, "%", 1, i);
                            screenWrite(screen, { sp[i].x - 4,sp[i].y + 1 }, "[%,%,%]", 3, (int)argObj.vertices[i].x, (int)argObj.vertices[i].y, (int)argObj.vertices[i].z);
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
        }
#pragma endregion
    }

    va_end(objlist);
}