#ifndef __V3DECORE_H__
#define __V3DECORE_H__

#include "vector3.h"

void RemoveCursor();

Vector2 GetCurrentCursorPos();

void SetCurrentCursorPos(Vector2 mousePos);

void resetTransform(Transform* transform);

void drawLine(char** screen, int screenWidth, int screenHeight, Vector2 p1, Vector2 p2);

Vector3 rotateVector(Vector3 center, Vector3 target, Vector3 rotation);

Vector3 lookAt(Vector3 pos, Vector3 target);

Vector2* worldVertex_Convert(int screenWidth, int screenHeight, Object obj, Camera camera);

void screenWrite(char** screen, Vector2 pos, const char* str, int varsize, ...);

void printScreen(char** screen, int screenWidth, int screenHeight, Vector2 RenderPos);

char** screenInit(int screenWidth, int screenHeight);

void Render(char** screen, int screenWidth, int screenHeight, Vector2 drawOffset, Camera camera, int GizmoType, int objArgs, ...);

#endif // !__V3DECORE_H__