#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#include "incndef.h"
#include "v3destruct.h"

Vector3 subtract(Vector3 a, Vector3 b);

float length(Vector3 v);

Vector3 normalize(Vector3 v);

double v3Distance(Vector3 v1, Vector3 v2);

#endif // !__VECTOR3_H__
