#include "vector3.h"

Vector3 subtract(Vector3 a, Vector3 b) {
    Vector3 result = { a.x - b.x, a.y - b.y, a.z - b.z };
    return result;
}

float length(Vector3 v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 normalize(Vector3 v) {
    float len = length(v);
    if (len > 0) {
        v.x /= len;
        v.y /= len;
        v.z /= len;
    }
    return v;
}

double v3Distance(Vector3 v1, Vector3 v2) {
    return sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.z, 2) + pow(v2.z - v1.z, 2));
}