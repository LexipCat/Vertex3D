#ifndef __V3DESTRUCT_H__
#define __V3DESTRUCT_H__

#define v3Zero {0,0,0}

typedef struct {
    int x, y;
} Vector2;

typedef struct {
    double x, y, z;

} Vector3;

typedef struct {
    //Vector3 worldPosition;
    Vector3 position, rotation;
}Transform;

typedef struct {
    Transform transform;
    float fov;
}Camera;

typedef struct {
    int a, b;
}Line;

typedef struct {
    Transform transform;
    Vector3* vertices;
    int isCustomChar;
    char* verteicesChar;
    Line* vertexLine;
    int v_Length, l_Length;
}Object;

#endif // !__V3DESTRUCT_H__
