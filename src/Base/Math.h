// https://www.youtube.com/watch?v=pThw0S8MR7w&t=3s
// https://www.youtube.com/watch?v=t7Ztio8cwqM
// https://www.youtube.com/watch?v=4p-E_31XOPM
// https://www.youtube.com/watch?v=PahbNFypubE&ab_channel=Bisqwit
// https://www.youtube.com/watch?v=hxOw_p0kLfI&t=346s&ab_channel=Bisqwit
// https://www.youtube.com/watch?v=NYW1TKZG-58&ab_channel=CedricMartens
// https://www.youtube.com/watch?v=f4s1h2YETNY&ab_channel=kishimisu
// https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/projection-matrix-GPU-rendering-pipeline-clipping.html
// https://www.mdpi.com/1999-4893/16/4/201
// https://www.braynzarsoft.net/viewtutorial/q16390-transformations-and-world-view-projection-space-matrices
// https://www.ijirt.org/master/publishedpaper/IJIRT100119_PAPER.pdf
// https://carmencincotti.com/2022-11-28/from-clip-space-to-ndc-space/
// https://groups.csail.mit.edu/graphics/classes/6.837/F04/lectures/07_Pipeline_II.pdf
// https://gabrielgambetta.com/computer-graphics-from-scratch/11-clipping.html
// https://www.youtube.com/watch?v=HXSuNxpCzdM&ab_channel=javidx9

#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

struct Vector2Int
{
    int x, y, z;
};
struct Vector2
{
    float x, y;
};
struct Vector3
{
    float x, y, z;
};
struct Vector4
{
    float x, y, z, w;
};
struct Triangle
{
    Vector3 p0, p1, p2;
};
struct Matrix
{
    float m[4][4];
};
struct Camera
{
    Vector3 position;
    float yaw;
    float pitch;
};
struct InputState
{
    bool w, a, s, d;
    bool left, up, down, right;
    bool e, q;
};

inline void Vector3Print(Vector3 v)
{
    // printf("<%f,%f,%f>", v.x, v.y, v.z);
    cout << "<" << v.x << "," << v.y << "," << v.z << ">" << endl;
}
inline void Vector4Print(Vector4 v)
{
    // printf("<%f,%f,%f,%f>", v.x, v.y, v.z, v.w);
    cout << "<" << v.x << "," << v.y << "," << v.z << "," << v.w << ">" << endl;
}
inline void MatrixPrint(Matrix m)
{
    cout << m.m[0][0] << "," << m.m[0][1] << "," << m.m[0][2] << "," << m.m[0][3] << endl;
    cout << m.m[1][0] << "," << m.m[1][1] << "," << m.m[1][2] << "," << m.m[1][3] << endl;
    cout << m.m[2][0] << "," << m.m[2][1] << "," << m.m[2][2] << "," << m.m[2][3] << endl;
    cout << m.m[3][0] << "," << m.m[3][1] << "," << m.m[3][2] << "," << m.m[3][3] << endl;
}

inline int MathSign(float value)
{
    if (value > 0) { return  1; }
    if (value < 0) { return -1; }
                     return  0;
}
inline int MathSign(int value)
{
    if (value > 0) { return  1; }
    if (value < 0) { return -1; }
                     return  0;
}
inline int MathMin(int v1, int v2)
{
    return min(v1, v2);
}
inline int MathMax(int v1, int v2)
{
    return max(v1, v2);
}
inline int MathClamp(int value, int min, int max)
{
    if (value < min) { return min; };
    if (value > max) { return max; };
                       return value;
}
inline float MathMin(float value1, float value2)
{
    return fminf(value1, value2);
}
inline float MathMax(float value1, float value2)
{
    return fmaxf(value1, value2);
}
inline float MathAbs(float value)
{
    return fabsf(value);
}
inline float MathSqrt(float value)
{
    return sqrtf(value);
}
inline float MathClamp(float value, float min, float max)
{
    if (value < min) { return min; };
    if (value > max) { return max; };
                       return value;
}
inline float MathSin(float f)
{
    return sinf(f);
}
inline float MathCos(float f)
{
    return cosf(f);
}
inline float MathInverseLerp(float x, float y, float value)
{
    return (value - x) / (y - x);
}
inline float MathMod(float value, float div)
{
    return fmodf(value, div);
}
inline float MathPingPong(float value, float length)
{
    value = MathAbs(value);
    int whole = (int)(value / length);
    float rem = fmodf(value, length);
    return whole % 2 == 0 ? rem : length - rem;
}
inline float MathToRadians(float degs)
{
    return degs * ((float)M_PI / 180.0f);
}
inline Vector3 Vector3Zero()
{
    return { 0, 0, 0 };
}
inline Vector3 Vector3Left()
{
    return { -1, 0, 0 };
}
inline Vector3 Vector3Right()
{
    return { 1, 0, 0 };
}
inline Vector3 Vector3Up()
{
    return { 0, 1, 0 };
}
inline Vector3 Vector3Down()
{
    return { 0, 1, 0 };
}
inline Vector3 Vector3Forward()
{
    return { 0, 0, 1 };
}
inline Vector3 Vector3Backward()
{
    return { 0, 0, -1 };
}
inline float Vector3Length(const Vector3& v)
{
    return MathSqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
inline float Vector3LengthSquared(Vector3 v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}
inline float Vector3Dot(Vector3 a, Vector3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
inline float Vector4Dot(Vector4 a, Vector4 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}
inline bool operator == (Vector3 left, Vector3 right)
{
    if (left.x != right.x) return false;
    if (left.y != right.y) return false;
    if (left.z != right.z) return false;
                           return true;
}
inline Vector3 operator - (Vector3 v)
{
    return { -v.x, -v.y, -v.z };
}
inline Vector3 operator + (Vector3 left, Vector3 right)
{
    left.x += right.x;
    left.y += right.y;
    left.z += right.z;
    return left;
}
inline Vector3 operator - (Vector3 left, Vector3 right)
{
    left.x -= right.x;
    left.y -= right.y;
    left.z -= right.z;
    return left;
}
inline Vector3 operator * (Vector3 v, float f)
{
    v.x *= f;
    v.y *= f;
    v.z *= f;
    return v;
}
inline Vector3 operator / (Vector3 v, float f)
{
    v.x /= f;
    v.y /= f;
    v.z /= f;
    return v;
}
inline Vector4 operator / (Vector4 v, float f)
{
    v.x /= f;
    v.y /= f;
    v.z /= f;
    v.w /= f;
    return v;
}
inline Vector3 operator += (Vector3& left, Vector3 right)
{
    left.x += right.x;
    left.y += right.y;
    left.z += right.z;
    return left;
}
inline Vector3 operator -= (Vector3& left, Vector3 right)
{
    left.x -= right.x;
    left.y -= right.y;
    left.z -= right.z;
    return left;
}
inline Vector3 operator *= (Vector3& v, float f)
{
    v.x *= f;
    v.y *= f;
    v.z *= f;
    return v;
}
inline Vector3 operator /= (Vector3& v, float f)
{
    v.x /= f;
    v.y /= f;
    v.z /= f;
    return v;
}
inline Vector3 Vector3ClampLength(const Vector3& v, float min, float max)
{
    auto length = Vector3Length(v);
    if (length > max) return v * (max / length);
    if (length < min) return v * (min / length);
    return v;
}
inline Vector3 Vector3Normalize(const Vector3& v)
{
    return v / Vector3Length(v);
}
inline Vector3 Vector3Cross(Vector3 a, Vector3 b)
{
    return
    {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}
inline bool Vector3TriangleIsClockwise(const Vector3& p1, const Vector3& p2, const Vector3& p3)
{
    auto v1 = p2 - p1;
    auto v2 = p3 - p1;
    auto crossZ = v1.x * v2.y - v1.y * v2.x;
    return crossZ < 0;
}
inline Vector3 Vector3MoveTowards(Vector3 fromVector, Vector3 toVector, float delta)
{
    if (fromVector == toVector) return fromVector;
    auto diff = toVector - fromVector;
    auto dist = Vector3Length(diff);
    if (dist <= delta) return toVector;
    diff = Vector3Normalize(diff);
    auto moveVec = diff * delta;
    return fromVector+moveVec;
}
inline Vector3 Vector3PositionUpdateSimple(Vector3 position, Vector3 velocity, float deltaTime)
{
    return position + velocity * deltaTime;
}
inline Vector3 Vector3PositionUpdateAdvanced(Vector3 position, Vector3 oldVelocity, Vector3 newVelocity, float deltaTime)
{
    return position + (oldVelocity+newVelocity) / 2 * deltaTime;
}
inline Matrix operator * (Matrix a, Matrix b)
{
    Vector4 row0 = { a.m[0][0], a.m[0][1], a.m[0][2], a.m[0][3] };
    Vector4 row1 = { a.m[1][0], a.m[1][1], a.m[1][2], a.m[1][3] };
    Vector4 row2 = { a.m[2][0], a.m[2][1], a.m[2][2], a.m[2][3] };
    Vector4 row3 = { a.m[3][0], a.m[3][1], a.m[3][2], a.m[3][3] };

    Vector4 col0 = { b.m[0][0], b.m[1][0], b.m[2][0], b.m[3][0] };
    Vector4 col1 = { b.m[0][1], b.m[1][1], b.m[2][1], b.m[3][1] };
    Vector4 col2 = { b.m[0][2], b.m[1][2], b.m[2][2], b.m[3][2] };
    Vector4 col3 = { b.m[0][3], b.m[1][3], b.m[2][3], b.m[3][3] };

    float m00 = Vector4Dot(row0, col0);
    float m01 = Vector4Dot(row0, col1);
    float m02 = Vector4Dot(row0, col2);
    float m03 = Vector4Dot(row0, col3);

    float m10 = Vector4Dot(row1, col0);
    float m11 = Vector4Dot(row1, col1);
    float m12 = Vector4Dot(row1, col2);
    float m13 = Vector4Dot(row1, col3);

    float m20 = Vector4Dot(row2, col0);
    float m21 = Vector4Dot(row2, col1);
    float m22 = Vector4Dot(row2, col2);
    float m23 = Vector4Dot(row2, col3);

    float m30 = Vector4Dot(row3, col0);
    float m31 = Vector4Dot(row3, col1);
    float m32 = Vector4Dot(row3, col2);
    float m33 = Vector4Dot(row3, col3);

    Matrix result =
    {
        m00, m01, m02, m03,
        m10, m11, m12, m13,
        m20, m21, m22, m23,
        m30, m31, m32, m33
    };

    return result;
}
inline Vector3 operator * (Vector3 v, Matrix m)
{
    Vector4 col0 = { m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0] };
    Vector4 col1 = { m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1] };
    Vector4 col2 = { m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2] };

    Vector4 row0 = { v.x, v.y, v.z, 1 };

    float x = Vector4Dot(row0,col0);
    float y = Vector4Dot(row0,col1);
    float z = Vector4Dot(row0,col2);

    return { x, y, z };
}
inline Vector4 operator * (Vector4 v, Matrix m)
{
    Vector4 col0 = { m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0] };
    Vector4 col1 = { m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1] };
    Vector4 col2 = { m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2] };
    Vector4 col3 = { m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3] };

    Vector4 row0 = { v.x, v.y, v.z, 1 };

    float x = Vector4Dot(row0, col0);
    float y = Vector4Dot(row0, col1);
    float z = Vector4Dot(row0, col2);
    float w = Vector4Dot(row0, col3);

    return { x, y, z, w };
}
inline Vector3 operator *= (Vector3& v, Matrix m)
{
    Vector4 col0 = { m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0] };
    Vector4 col1 = { m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1] };
    Vector4 col2 = { m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2] };

    Vector4 row0 = { v.x, v.y, v.z, 1 };

    float x = Vector4Dot(row0, col0);
    float y = Vector4Dot(row0, col1);
    float z = Vector4Dot(row0, col2);

    v = { x, y, z };

    return v;
}
inline Matrix MatrixIdentity()
{
    return
    {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
    };
}
inline Matrix MatrixTranspose(Matrix m)
{
    return
    {
        m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0],
        m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1],
        m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2],
        m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3]
    };
}
inline Matrix MatrixTranslate(Vector3 v)
{
    return
    {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        v.x, v.y, v.z, 1
    };
}
inline Matrix MatrixRotateX(float rad)
{
    float sin = sinf(rad);
    float cos = cosf(rad);
    return
    {
        1,   0,    0,    0,
        0,   cos, -sin,  0,
        0,   sin,  cos,  0,
        0,   0,    0,    1
    };
}
inline Matrix MatrixRotateY(float rad)
{
    float sin = sinf(rad);
    float cos = cosf(rad);
    return
    {
        cos,  0,  -sin,  0,
        0,    1,   0,    0,
        sin,  0,   cos,  0,
        0,    0,   0,    1
    };
}
inline Matrix MatrixRotateZ(float rad)
{
    float sin = sinf(rad);
    float cos = cosf(rad);
    return
    {
        cos, -sin,  0,   0,
        sin,  cos,  0,   0,
        0,    0,    1,   0,
        0,    0,    0,   1
    };
}
inline Matrix MatrixRotate(Vector3 rotation)
{
    return
    MatrixRotateX(MathToRadians(rotation.x)) *
    MatrixRotateY(MathToRadians(rotation.y)) *
    MatrixRotateZ(MathToRadians(rotation.z));
}
inline Matrix MatrixScale(Vector3 scale)
{
    float x = scale.x;
    float y = scale.y;
    float z = scale.z;
    return
    {
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1
    };
}
inline Matrix MatrixWorld(Vector3 position, Vector3 direction)
{
    Vector3 zAxis = direction;
            zAxis = Vector3Normalize(zAxis); // maybe remove

    Vector3 xAxis = Vector3Cross(Vector3Up(), zAxis);
            xAxis = Vector3Normalize(xAxis);

    Vector3 yAxis = Vector3Cross(zAxis, xAxis);

    auto x = position.x;
    auto y = position.y;
    auto z = position.z;

    return
    {
        xAxis.x, xAxis.y, xAxis.z, 0.0f,
        yAxis.x, yAxis.y, yAxis.z, 0.0f,
        zAxis.x, zAxis.y, zAxis.z, 0.0f,
              x,       y,       z, 1.0f
    };
}
inline Matrix MatrixWorld(Vector3 position, Vector3 rotation, Vector3 scale)
{
    return
    MatrixScale(scale) *
    MatrixRotate(rotation) *
    MatrixTranslate(position);
}
inline Matrix MatrixView(Vector3 eye, float yaw, float pitch)
{
    return
    MatrixTranslate(-eye) *
    MatrixRotateY(-yaw) *
    MatrixRotateX(-pitch);
}
inline Matrix MatrixView(Vector3 eye, Vector3 target, Vector3 up)
{
    Vector3 zAxis = target - eye;
            zAxis = Vector3Normalize(zAxis);

    Vector3 xAxis = Vector3Cross(up, zAxis);
            xAxis = Vector3Normalize(xAxis);

    Vector3 yAxis = Vector3Cross(zAxis, xAxis);

    auto x = -Vector3Dot(xAxis, eye);
    auto y = -Vector3Dot(yAxis, eye);
    auto z = -Vector3Dot(zAxis, eye);

    return
    {
        xAxis.x, xAxis.y, xAxis.z, 0.0f,
        yAxis.x, yAxis.y, yAxis.z, 0.0f,
        zAxis.x, zAxis.y, zAxis.z, 0.0f,
              x,       y,       z, 1.0f
    };
}
inline Matrix MatrixView(const Camera& camera)
{
    return MatrixView(camera.position, camera.yaw, camera.pitch);
}
inline Matrix MatrixOrthographic(float width, float height, float zNear, float zFar)
{
    float w = 2.0f / width;
    float h = 2.0f / height;
    float a = 1.0f / (zFar - zNear);
    float b = a * -zNear;

    return
    {
        w, 0, 0, 0,
        0, h, 0, 0,
        0, 0, a, 0,
        0, 0, b, 1
    };
}
inline Matrix MatrixPerspective(float width, float height, float zNear, float zFar)
{
    float aspectRatio = width / height;
    float fov = (float)(M_PI_2);
    float h = 1.0f / tanf(fov / 2);
    float w = h / aspectRatio;
    float a = zFar / (zFar - zNear);
    float b = a * -zNear;

    return
    {
        w, 0, 0, 0,
        0, h, 0, 0,
        0, 0, a, 1,
        0, 0, b, 0
    };
}
inline void UpdateCameraRotation(Camera& camera, const InputState& inputState, float deltaTime)
{
    float speed = (float)M_PI;
    float speedDelta = speed * deltaTime;
    if (inputState.up)    camera.pitch += speedDelta;
    if (inputState.down)  camera.pitch -= speedDelta;
    if (inputState.right) camera.yaw   += speedDelta;
    if (inputState.left)  camera.yaw   -= speedDelta;

    // TODO review
    // Wrap yaw to avoid floating-point errors if we turn too far
    float M_PI2 = 2 * (float)M_PI;
    while (camera.yaw >=  M_PI2) camera.yaw -= M_PI2;
    while (camera.yaw <= -M_PI2) camera.yaw += M_PI2;

    // Clamp pitch to stop camera flipping upside down
    float degree = MathToRadians(85);
    if (camera.pitch >  degree) camera.pitch =  degree;
    if (camera.pitch < -degree) camera.pitch = -degree;
}
inline void UpdateCameraPosition(Camera& camera, const InputState& inputState, float deltaTime)
{
    auto matrix = MatrixView(camera);
    Vector3 forward = { matrix.m[0][2], matrix.m[1][2], matrix.m[2][2] };
    Vector3 up = Vector3Up();
    Vector3 right = Vector3Cross(up, forward);
    auto speed = 50.0f;
    auto speedDelta = speed * deltaTime;

    if (inputState.w) camera.position += forward * speedDelta;
    if (inputState.s) camera.position -= forward * speedDelta;
    if (inputState.d) camera.position += right   * speedDelta;
    if (inputState.a) camera.position -= right   * speedDelta;
    if (inputState.e) camera.position += up      * speedDelta;
    if (inputState.q) camera.position -= up      * speedDelta;
}
inline bool InFrustum(Vector3 point)
{
    if (point.z / MathAbs(point.x) < 1) return false;
    if (point.z / MathAbs(point.y) < 1) return false;
    return true;
}
