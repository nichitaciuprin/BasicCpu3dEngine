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

#define MY_PI        (float)3.14159265358979323846
#define MY_PI_MUL_2  (float)6.28318530717958647692
#define MY_PI_DIV_2  (float)1.57079632679489661923

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
struct Vector2Int
{
    int x, y;
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
struct Bound
{
    Vector3 p0;
    Vector3 p1;
};
struct Box
{
    Vector3 position;
    Vector3 rotation;
    Vector3 halfExtent;
};
struct Sphere
{
    Vector3 position;
    float radius;
};
struct Pose
{
    Vector3 position;
    Vector3 rotation;
};

inline void PrintFloatHex(float f)
{
    unsigned int ui;
    memcpy(&ui, &f, sizeof (ui));
    printf("%X", ui);
}
inline void PrintVector2(Vector2 v)
{
    cout << "<" << v.x << "," << v.y << ">" << endl;
}
inline void PrintVector3(Vector3 v)
{
    cout << "<" << v.x << "," << v.y << "," << v.z << ">" << endl;
}
inline void PrintVector4(Vector4 v)
{
    cout << "<" << v.x << "," << v.y << "," << v.z << "," << v.w << ">" << endl;
}
inline void PrintVector2Hex(Vector2 v)
{
    PrintFloatHex(v.x);
    PrintFloatHex(v.y);
}
inline void PrintMatrix(Matrix m)
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
    return min(v1,v2);
}
inline int MathMax(int v1, int v2)
{
    return max(v1,v2);
}
inline int MathClamp(int value, int min, int max)
{
    if (value < min) { return min; };
    if (value > max) { return max; };
                       return value;
}
inline float MathMin(float value1, float value2)
{
    return fminf(value1,value2);
}
inline float MathMax(float value1, float value2)
{
    return fmaxf(value1,value2);
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
    return (value - x)/(y - x);
}
inline float MathMod(float value, float div)
{
    return fmodf(value,div);
}
inline float MathPingPong(float value, float length)
{
    value = MathAbs(value);
    int whole = (int)(value/length);
    float rem = fmodf(value,length);
    return whole % 2 == 0 ? rem : length-rem;
}
inline float MathToDegree(float radians)
{
    float _180_div_pi = (float)57.2957795130823208768;
    return _180_div_pi * radians;
}
inline float MathToRadians(float degree)
{
    float _pi_div_180 = (float)0.01745329251994329576;
    return _pi_div_180 * degree;
}

inline bool Vector2Equal(Vector2 v1, Vector2 v2)
{
    return v1.x == v2.x && v1.y == v2.y;
}
inline Vector2 Vector2Abs(Vector2 v)
{
    return { fabs(v.x), fabs(v.y) };
}
inline Vector2 Vector2Add(Vector2 v1, Vector2 v2)
{
    return { v1.x + v2.x, v1.y + v2.y };
}
inline Vector2 Vector2Sub(Vector2 v1, Vector2 v2)
{
    return { v1.x - v2.x, v1.y - v2.y };
}
inline Vector2 Vector2Mul(Vector2 v1, float value)
{
    return { v1.x * value, v1.y * value };
}
inline Vector2 Vector2Div(Vector2 v1, float value)
{
    return { v1.x / value, v1.y / value };
}
inline float Vector2Length(Vector2 v)
{
    return MathSqrt(v.x*v.x + v.y*v.y);
}
inline float Vector2Rotation(Vector2 v)
{
    // atan2( 0.0f,  1.0f ) = 0.0000
    // atan2( 1.0f,  1.0f ) = 0.7853
    // atan2( 1.0f,  0.0f ) = 1.5708
    // atan2( 1.0f, -1.0f ) = 2.3561
    // atan2( 0.0f, -1.0f ) = 3.1415
    return atan2(v.x, v.y);
}
inline Vector2 Vector2Normalize(Vector2 v)
{
    float length = Vector2Length(v);

    if (length > 0)
        return Vector2Div(v, length);
    else
        return { 0, 0 };
}
inline Vector2 Vector2MoveTowards(Vector2 from, Vector2 target, float delta)
{
    if (Vector2Equal(from, target)) return from;
    auto diff = Vector2Sub(target, from);
    auto dist = Vector2Length(diff);
    if (dist <= delta) return target;
    diff = Vector2Normalize(diff);
    auto moveVec = Vector2Mul(diff, delta);
    return Vector2Add(from, moveVec);
}
inline Vector2 Vector2MoveTowards(Vector2 from, Vector2 target, float delta, float limit)
{
    if (Vector2Equal(from, target)) return from;
    auto diff = Vector2Sub(target, from);
    auto dist = Vector2Length(diff);
    if (dist <= limit) return from;
    if (dist <= delta) return target;
    diff = Vector2Normalize(diff);
    auto moveVec = Vector2Mul(diff, delta);
    return Vector2Add(from, moveVec);
}

inline bool Vector3Equal(Vector3 v1, Vector3 v2)
{
    return
    v1.x == v2.x &&
    v1.y == v2.y &&
    v1.z == v2.z;
}
inline Vector3 Vector3Abs(Vector3 v)
{
    return { fabs(v.x), fabs(v.y), fabs(v.z) };
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
inline Vector3 Vector3Negative(Vector3 v)
{
    return { -v.x, -v.y, -v.z };
}
inline Vector3 Vector3Add(Vector3 v1, Vector3 v2)
{
    return { v1.x+v2.x, v1.y+v2.y, v1.z+v2.z };
}
inline Vector3 Vector3Subtract(Vector3 v1, Vector3 v2)
{
    return { v1.x-v2.x, v1.y-v2.y, v1.z-v2.z };
}
inline Vector3 Vector3Divide(Vector3 v, float f)
{
    return { v.x/f, v.y/f, v.z/f };
}
inline Vector3 Vector3Multiply(Vector3 v, float f)
{
    return { v.x*f, v.y*f, v.z*f };
}
inline float Vector3Length(Vector3 v)
{
    return MathSqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}
inline float Vector3LengthSquared(Vector3 v)
{
    return v.x*v.x + v.y*v.y + v.z*v.z;
}
inline float Vector3Dot(Vector3 a, Vector3 b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}
inline float Vector3Distance(Vector3 a, Vector3 b)
{
    auto diff = Vector3Subtract(a, b);
    return Vector3Length(diff);
}
inline Vector3 Vector3ClampLength(Vector3 vector, float min, float max)
{
    auto length = Vector3Length(vector);
    if (length <= 0) return { 0, 0, 0 };
    if (length > max) return Vector3Multiply(vector, (max / length));
    if (length < min) return Vector3Multiply(vector, (min / length));
    return vector;
}
inline Vector3 Vector3Normalize(Vector3 v)
{
    float length = Vector3Length(v);
    if (length <= 0) return { 0, 0 };
    return Vector3Divide(v, length);
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
inline Vector3 Vector3RotateX(Vector3 v, float rot)
{
    float sin = sinf(rot);
    float cos = cosf(rot);
    float y = v.y * ( cos) + v.z * ( sin);
    float z = v.y * (-sin) + v.z * ( cos);
    return { v.x, y, z };
}
inline Vector3 Vector3RotateY(Vector3 v, float rot)
{
    float sin = sinf(rot);
    float cos = cosf(rot);
    float x = v.x * ( cos) + v.z * (-sin);
    float z = v.x * ( sin) + v.z * ( cos);
    return { x, v.y, z };
}
inline Vector3 Vector3RotateZ(Vector3 v, float rot)
{
    float sin = sinf(rot);
    float cos = cosf(rot);
    float x = v.x * ( cos) + v.y * ( sin);
    float y = v.x * (-sin) + v.y * ( cos);
    return { x, y, v.z };
}
inline Vector3 Vector3Rotate(Vector3 v, Vector3 rot)
{
    v = Vector3RotateX(v, rot.x);
    v = Vector3RotateY(v, rot.y);
    v = Vector3RotateZ(v, rot.z);
    return v;
}
inline Vector3 Vector3MoveTowards(Vector3 source, Vector3 target, float delta)
{
    if (Vector3Equal(source, target)) return source;
    auto diff = Vector3Subtract(target, source);
    auto dist = Vector3Length(diff);
    if (dist <= delta) return target;
    diff = Vector3Normalize(diff);
    auto moveVec = Vector3Multiply(diff, delta);
    return Vector3Add(source, moveVec);
}
inline Vector3 Vector3MoveTowards2(Vector3 source, Vector3 target, float acc, float deltaTime)
{
    // like easeOut function

    float dist = Vector3Distance(source, target);
    float speed = MathSqrt(2 * dist * acc);
    return Vector3MoveTowards(source, target, speed * deltaTime);
}
inline Vector3 Vector3MoveTowards3(Vector3 source, Vector3 target, Vector3* velocity, float acc, float maxSpeed, float deltaTime)
{
    // like easeInOut function
    // but source will decelerate only on rotations

    Vector3 diff = Vector3Subtract(target, source);
    Vector3 dir = Vector3Normalize(diff);

    Vector3 targetVelocity = Vector3Multiply(dir, maxSpeed);

    Vector3 oldVelocity = *velocity;
    Vector3 newVelocity = Vector3MoveTowards(oldVelocity, targetVelocity, acc * deltaTime);

    Vector3 offset;

    offset = Vector3Add(oldVelocity, newVelocity);
    offset = Vector3Divide(offset, 2);
    offset = Vector3Multiply(offset, deltaTime);

    Vector3 result = Vector3Add(source, offset);

    *velocity = newVelocity;

    return result;
}
inline Vector3 Vector3MoveTowards4(Vector3 source, Vector3 target, Vector3* velocity, float acc, float deltaTime)
{
    // like easeInOut function

    Vector3 diff = Vector3Subtract(target, source);
    Vector3 dir = Vector3Normalize(diff);

    float dist = Vector3Length(diff);
    float maxSpeed = MathSqrt(2 * dist * acc);

    Vector3 targetVelocity = Vector3Multiply(dir, maxSpeed);

    Vector3 oldVelocity = *velocity;
    Vector3 newVelocity = Vector3MoveTowards(oldVelocity, targetVelocity, acc * deltaTime);

    Vector3 offset;

    offset = Vector3Add(oldVelocity, newVelocity);
    offset = Vector3Divide(offset, 2);
    offset = Vector3Multiply(offset, deltaTime);

    Vector3 result = Vector3Add(source, offset);

    *velocity = newVelocity;

    return result;
}
inline Vector3 Vector3MoveTowards5(Vector3 source, Vector3 target, Vector3* velocity, float acc, float deltaTime)
{
    // like easeInOut function
    // but removes jittering near target

    Vector3 diff = Vector3Subtract(target, source);
    Vector3 dir = Vector3Normalize(diff);

    float dist = Vector3Length(diff);

    float maxSpeed = MathSqrt(2 * dist * acc);
    Vector3 targetVelocity = Vector3Multiply(dir, maxSpeed);

    Vector3 oldVelocity = *velocity;
    Vector3 newVelocity;

    if (dist < 0.1f)
        newVelocity = targetVelocity;
    else
        newVelocity = Vector3MoveTowards(oldVelocity, targetVelocity, acc * deltaTime);

    Vector3 offset;

    offset = Vector3Add(oldVelocity, newVelocity);
    offset = Vector3Divide(offset, 2);
    offset = Vector3Multiply(offset, deltaTime);

    Vector3 result = Vector3Add(source, offset);

    *velocity = newVelocity;

    return result;
}

inline float Vector4Dot(Vector4 a, Vector4 b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

inline Matrix MatrixIdentity()
{
    return
    {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1,
    };
}
inline Matrix MatrixMultiply(Matrix a, Matrix b)
{
    Vector4 row0 = { a.m[0][0], a.m[0][1], a.m[0][2], a.m[0][3] };
    Vector4 row1 = { a.m[1][0], a.m[1][1], a.m[1][2], a.m[1][3] };
    Vector4 row2 = { a.m[2][0], a.m[2][1], a.m[2][2], a.m[2][3] };
    Vector4 row3 = { a.m[3][0], a.m[3][1], a.m[3][2], a.m[3][3] };

    Vector4 col0 = { b.m[0][0], b.m[1][0], b.m[2][0], b.m[3][0] };
    Vector4 col1 = { b.m[0][1], b.m[1][1], b.m[2][1], b.m[3][1] };
    Vector4 col2 = { b.m[0][2], b.m[1][2], b.m[2][2], b.m[3][2] };
    Vector4 col3 = { b.m[0][3], b.m[1][3], b.m[2][3], b.m[3][3] };

    float m00 = Vector4Dot(row0,col0);
    float m01 = Vector4Dot(row0,col1);
    float m02 = Vector4Dot(row0,col2);
    float m03 = Vector4Dot(row0,col3);

    float m10 = Vector4Dot(row1,col0);
    float m11 = Vector4Dot(row1,col1);
    float m12 = Vector4Dot(row1,col2);
    float m13 = Vector4Dot(row1,col3);

    float m20 = Vector4Dot(row2,col0);
    float m21 = Vector4Dot(row2,col1);
    float m22 = Vector4Dot(row2,col2);
    float m23 = Vector4Dot(row2,col3);

    float m30 = Vector4Dot(row3,col0);
    float m31 = Vector4Dot(row3,col1);
    float m32 = Vector4Dot(row3,col2);
    float m33 = Vector4Dot(row3,col3);

    Matrix result =
    {
        m00,m01,m02,m03,
        m10,m11,m12,m13,
        m20,m21,m22,m23,
        m30,m31,m32,m33
    };

    return result;
}
inline Matrix MatrixTranspose(Matrix a)
{
    return Matrix
    {
        a.m[0][0],a.m[1][0],a.m[2][0],a.m[3][0],
        a.m[0][1],a.m[1][1],a.m[2][1],a.m[3][1],
        a.m[0][2],a.m[1][2],a.m[2][2],a.m[3][2],
        a.m[0][3],a.m[1][3],a.m[2][3],a.m[3][3]
    };
}
inline Matrix MatrixInvert(Matrix mat)
{
    Matrix result;

    float a00 = mat.m[0][0], a01 = mat.m[0][1], a02 = mat.m[0][2], a03 = mat.m[0][3];
    float a10 = mat.m[1][0], a11 = mat.m[1][1], a12 = mat.m[1][2], a13 = mat.m[1][3];
    float a20 = mat.m[2][0], a21 = mat.m[2][1], a22 = mat.m[2][2], a23 = mat.m[2][3];
    float a30 = mat.m[3][0], a31 = mat.m[3][1], a32 = mat.m[3][2], a33 = mat.m[3][3];

    float b00 = a00*a11 - a01*a10;
    float b01 = a00*a12 - a02*a10;
    float b02 = a00*a13 - a03*a10;
    float b03 = a01*a12 - a02*a11;
    float b04 = a01*a13 - a03*a11;
    float b05 = a02*a13 - a03*a12;
    float b06 = a20*a31 - a21*a30;
    float b07 = a20*a32 - a22*a30;
    float b08 = a20*a33 - a23*a30;
    float b09 = a21*a32 - a22*a31;
    float b10 = a21*a33 - a23*a31;
    float b11 = a22*a33 - a23*a32;

    float invDet = 1.0f/(b00*b11 - b01*b10 + b02*b09 + b03*b08 - b04*b07 + b05*b06);

    result.m[0][0] = ( a11*b11 - a12*b10 + a13*b09)*invDet;
    result.m[0][1] = (-a01*b11 + a02*b10 - a03*b09)*invDet;
    result.m[0][2] = ( a31*b05 - a32*b04 + a33*b03)*invDet;
    result.m[0][3] = (-a21*b05 + a22*b04 - a23*b03)*invDet;
    result.m[1][0] = (-a10*b11 + a12*b08 - a13*b07)*invDet;
    result.m[1][1] = ( a00*b11 - a02*b08 + a03*b07)*invDet;
    result.m[1][2] = (-a30*b05 + a32*b02 - a33*b01)*invDet;
    result.m[1][3] = ( a20*b05 - a22*b02 + a23*b01)*invDet;
    result.m[2][0] = ( a10*b10 - a11*b08 + a13*b06)*invDet;
    result.m[2][1] = (-a00*b10 + a01*b08 - a03*b06)*invDet;
    result.m[2][2] = ( a30*b04 - a31*b02 + a33*b00)*invDet;
    result.m[2][3] = (-a20*b04 + a21*b02 - a23*b00)*invDet;
    result.m[3][0] = (-a10*b09 + a11*b07 - a12*b06)*invDet;
    result.m[3][1] = ( a00*b09 - a01*b07 + a02*b06)*invDet;
    result.m[3][2] = (-a30*b03 + a31*b01 - a32*b00)*invDet;
    result.m[3][3] = ( a20*b03 - a21*b01 + a22*b00)*invDet;

    return result;
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
         cos,   0,  sin,   0,
           0,   1,    0,   0,
        -sin,   0,  cos,   0,
           0,   0,    0,   1
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
    Matrix result;
    Matrix matX = MatrixRotateX(rotation.x);
    Matrix matY = MatrixRotateY(rotation.y);
    Matrix matZ = MatrixRotateZ(rotation.z);
    result = matX;
    result = MatrixMultiply(result, matY);
    result = MatrixMultiply(result, matZ);
    return result;
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
    Matrix result;
    Matrix mat1 = MatrixScale(scale);
    Matrix mat2 = MatrixRotate(rotation);
    Matrix mat3 = MatrixTranslate(position);
    result = mat1;
    result = MatrixMultiply(result, mat2);
    result = MatrixMultiply(result, mat3);
    return result;
}
inline Matrix MatrixView(Vector3 eye, float yaw, float pitch)
{
    eye = Vector3Negative(eye);
    Matrix mat1 = MatrixTranslate(eye);
    // TODO
    Matrix mat2 = MatrixRotateY(yaw);
    Matrix mat3 = MatrixRotateX(-pitch);
    Matrix result;
    result = mat1;
    result = MatrixMultiply(result, mat2);
    result = MatrixMultiply(result, mat3);
    return result;
}
inline Matrix MatrixView(Vector3 eye, Vector3 target, Vector3 up)
{
    Vector3 zAxis = Vector3Subtract(target, eye);
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
inline Matrix MatrixView(const Camera* camera)
{
    return MatrixView(camera->position,camera->yaw,camera->pitch);
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
    float fov = (float)(MY_PI_DIV_2);
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

inline Vector3 ToEuler(Vector4 q)
{
    Vector3 result;

    const float x = q.x;
    const float y = q.y;
    const float z = q.z;
    const float w = q.w;

    float x0 = 2.0f*(w*x + y*z);
    float x1 = 1.0f - 2.0f*(x*x + y*y);
    result.x = atan2f(x0, x1);

    float y0 = 2.0f*(w*y - z*x);
    y0 = y0 >  1.0f ?  1.0f : y0;
    y0 = y0 < -1.0f ? -1.0f : y0;
    result.y = asinf(y0);

    float z0 = 2.0f*(w*z + x*y);
    float z1 = 1.0f - 2.0f*(y*y + z*z);
    result.z = atan2f(z0, z1);

    return result;
}
inline Vector4 ToQuaternion(Vector3 e)
{
    Vector4 result;

    auto pitch = e.x /= 2;
    auto yaw   = e.y /= 2;
    auto roll  = e.z /= 2;

    float x0 = cosf(pitch);
    float x1 = sinf(pitch);
    float y0 = cosf(yaw);
    float y1 = sinf(yaw);
    float z0 = cosf(roll);
    float z1 = sinf(roll);

    result.x = x1*y0*z0 - x0*y1*z1;
    result.y = x0*y1*z0 + x1*y0*z1;
    result.z = x0*y0*z1 - x1*y1*z0;
    result.w = x0*y0*z0 + x1*y1*z1;

    return result;
}

inline bool Vector3TriangleIsClockwise(Vector3 p1, Vector3 p2, Vector3 p3)
{
    auto v1 = Vector3Subtract(p2, p1);
    auto v2 = Vector3Subtract(p3, p1);
    auto crossZ = v1.x * v2.y - v1.y * v2.x;
    return crossZ < 0;
}

inline Vector3 GetAxisX(Camera* camera)
{
    Vector3 result = { 1, 0, 0 };
    result = Vector3RotateX(result, camera->pitch);
    result = Vector3RotateY(result, -camera->yaw);
    return result;
}
inline Vector3 GetAxisY(Camera* camera)
{
    Vector3 result = { 0, 1, 0 };
    result = Vector3RotateX(result, camera->pitch);
    result = Vector3RotateY(result, -camera->yaw);
    return result;
}
inline Vector3 GetAxisZ(Camera* camera)
{
    Vector3 result = { 0, 0, 1 };
    result = Vector3RotateX(result, camera->pitch);
    result = Vector3RotateY(result, -camera->yaw);
    return result;
}

inline bool InFrustum(Vector3 point)
{
    if (point.z / MathAbs(point.x) < 1) return false;
    if (point.z / MathAbs(point.y) < 1) return false;
    return true;
}

inline void UpdateCameraRotation(Camera* camera, float deltaTime, bool left, bool up, bool down, bool right)
{
    float speed = (float)MY_PI;
    float speedDelta = speed * deltaTime;
    if (up)    camera->pitch += speedDelta;
    if (down)  camera->pitch -= speedDelta;
    if (right) camera->yaw   += speedDelta;
    if (left)  camera->yaw   -= speedDelta;

    // TODO review
    // Wrap yaw to avoid floating-point errors if we turn too far
    while (camera->yaw >=  MY_PI_MUL_2) camera->yaw -= MY_PI_MUL_2;
    while (camera->yaw <= -MY_PI_MUL_2) camera->yaw += MY_PI_MUL_2;

    // Clamp pitch to stop camera flipping upside down
    float degree = MathToRadians(85);
    if (camera->pitch >  degree) camera->pitch =  degree;
    if (camera->pitch < -degree) camera->pitch = -degree;
}
inline void UpdateCameraRotation(Camera* camera, float rotX, float rotY)
{
    // inverses horizontal rotation when upsidedown
    // makes movement easy
    // float angle = cos(camera->pitch);
    // if (angle < 0)
    //     rotX = -rotX;

    camera->yaw += rotX;
    camera->pitch += rotY;
}
inline void UpdateCameraPosition(Camera* camera, float deltaTime, bool w, bool a, bool s, bool d, bool e, bool q)
{
    Matrix matrix = MatrixView(camera);

    Vector3 forward = { matrix.m[0][2], matrix.m[1][2], matrix.m[2][2] };
    Vector3 up = Vector3Up();
    Vector3 right = Vector3Cross(up,forward);

    auto speed = 50.0f;
    auto speedDelta = speed * deltaTime;

    forward = Vector3Multiply(forward, speedDelta);
    right   = Vector3Multiply(right, speedDelta);
    up      = Vector3Multiply(up, speedDelta);

    if (w) camera->position = Vector3Add      (camera->position, forward);
    if (s) camera->position = Vector3Subtract (camera->position, forward);
    if (d) camera->position = Vector3Add      (camera->position, right);
    if (a) camera->position = Vector3Subtract (camera->position, right);
    if (e) camera->position = Vector3Add      (camera->position, up);
    if (q) camera->position = Vector3Subtract (camera->position, up);
}
inline void UpdateCameraPosition(Camera* camera, Vector3 move)
{
    Vector3 forward = GetAxisZ(camera);
    Vector3 up = GetAxisY(camera);
    Vector3 right = Vector3Cross(up, forward);
    right = Vector3Normalize(right);
    up = Vector3Cross(forward, right);

    forward = { forward.x*move.z, forward.y*move.z, forward.z*move.z };
    up = { up.x*move.y, up.y*move.y, up.z*move.y };
    right = { right.x*move.x, right.y*move.x, right.z*move.x };

    camera->position = Vector3Add(camera->position, forward)    ;
    camera->position = Vector3Add(camera->position, up);
    camera->position = Vector3Add(camera->position, right);
}

inline void UpdateCameraFpsRotation(Camera& camera, Vector2 rot)
{
    camera.yaw += rot.x;
    camera.pitch += rot.y;

    // maybe bigger angle?
    // camera.pitch = MathClamp(camera.pitch, MathToRadians(-120.0f), MathToRadians(120.0f));

    camera.pitch = MathClamp(camera.pitch, MathToRadians(-90.0f), MathToRadians(90.0f));
}
inline void UpdateCameraFpsMovement(Camera& camera, Vector2 move)
{
    Vector3 up = { 0, 1, 0 };
    Vector3 forward = { 0, 0, 1 };

    forward = Vector3RotateY(forward, -camera.yaw);

    Vector3 right = Vector3Cross(up, forward);

    forward.x *= move.y;
    forward.z *= move.y;
    right.x *= move.x;
    right.z *= move.x;

    camera.position = Vector3Add(camera.position, forward);
    camera.position = Vector3Add(camera.position, right);
}
inline void UpdateCameraFps(Camera& camera, Vector2 move, Vector2 rot)
{
    UpdateCameraFpsRotation(camera, rot);
    UpdateCameraFpsMovement(camera, move);
}

inline float BoundMinX(Bound* bound)
{
    return fminf(bound->p0.x, bound->p1.x);
}
inline float BoundMaxX(Bound* bound)
{
    return fmaxf(bound->p0.x, bound->p1.x);
}
inline float BoundMinY(Bound* bound)
{
    return fminf(bound->p0.y, bound->p1.y);
}
inline float BoundMaxY(Bound* bound)
{
    return fmaxf(bound->p0.y, bound->p1.y);
}
inline float BoundMinZ(Bound* bound)
{
    return fminf(bound->p0.z, bound->p1.z);
}
inline float BoundMaxZ(Bound* bound)
{
    return fmaxf(bound->p0.z, bound->p1.z);
}
inline Vector3 BoundSize(const Bound* bound)
{
    Vector3 result = Vector3Subtract(bound->p0, bound->p1);
    result.x = MathAbs(result.x);
    result.y = MathAbs(result.y);
    result.z = MathAbs(result.z);
    return result;
}
inline Vector3 BoundCenter(const Bound& bound)
{
    Vector3 result;
    result = Vector3Add(bound.p0, bound.p1);
    result = Vector3Divide(result, 2);
    return result;
}

inline Vector3 RandomPointInside(Subgen* subgen, Bound* bound)
{
    float x = SubgenRange(subgen, BoundMinX(bound), BoundMaxX(bound) );
    float y = SubgenRange(subgen, BoundMinY(bound), BoundMaxY(bound) );
    float z = SubgenRange(subgen, BoundMinZ(bound), BoundMaxZ(bound) );
    return { x, y, z };
}
inline Vector3 RandomPointTopFace(Subgen* subgen, Bound* bound)
{
    auto maxY = BoundMaxY(bound);

    auto minX = BoundMinX(bound);
    auto maxX = BoundMaxX(bound);

    auto minZ = BoundMinZ(bound);
    auto maxZ = BoundMaxZ(bound);

    auto randX = SubgenRange(subgen, minX, maxX);
    auto randZ = SubgenRange(subgen, minZ, maxZ);

    return { randX, maxY, randZ };
}
inline Vector3 WrapAround(Bound* bound, Vector3 point)
{
    Vector3 size = BoundSize(bound);
    if      (point.x < BoundMinX(bound)) point.x += size.x;
    else if (point.x > BoundMaxX(bound)) point.x -= size.x;
    if      (point.y < BoundMinY(bound)) point.y += size.y;
    else if (point.y > BoundMaxY(bound)) point.y -= size.y;
    if      (point.z < BoundMinZ(bound)) point.z += size.z;
    else if (point.z > BoundMaxZ(bound)) point.z -= size.z;
    return point;
}
inline Vector3 BoundShortPathIn(Bound* aabb, Vector3 point)
{
    auto result = Vector3Zero();
    if      (point.x < BoundMinX(aabb)) result.x = BoundMinX(aabb) - point.x;
    else if (point.x > BoundMaxX(aabb)) result.x = BoundMaxX(aabb) - point.x;
    if      (point.y < BoundMinY(aabb)) result.y = BoundMinY(aabb) - point.y;
    else if (point.y > BoundMaxY(aabb)) result.y = BoundMaxY(aabb) - point.y;
    if      (point.z < BoundMinZ(aabb)) result.z = BoundMinZ(aabb) - point.z;
    else if (point.z > BoundMaxZ(aabb)) result.z = BoundMaxZ(aabb) - point.z;
    return result;
}

inline Vector2 operator -  (Vector2 left, Vector2 right)
{
    left.x -= right.x;
    left.y -= right.y;
    return left;
}
inline Vector2 operator *  (Vector2 v, float f)
{
    v.x *= f;
    v.y *= f;
    return v;
}
inline Vector2 operator /  (Vector2 v, float f)
{
    v.x /= f;
    v.y /= f;
    return v;
}
inline Vector2 operator /= (Vector2& v, float f)
{
    v.x /= f;
    v.y /= f;
    return v;
}

inline bool    operator == (Vector3 left, Vector3 right)
{
    if (left.x != right.x) { return false; };
    if (left.y != right.y) { return false; };
    if (left.z != right.z) { return false; };
                             return true;
}
inline Vector3 operator -  (Vector3 v)
{
    return {-v.x, -v.y, -v.z};
}
inline Vector3 operator +  (Vector3 left, Vector3 right)
{
    left.x += right.x;
    left.y += right.y;
    left.z += right.z;
    return left;
}
inline Vector3 operator -  (Vector3 left, Vector3 right)
{
    left.x -= right.x;
    left.y -= right.y;
    left.z -= right.z;
    return left;
}
inline Vector3 operator *  (Vector3 v, float f)
{
    v.x *= f;
    v.y *= f;
    v.z *= f;
    return v;
}
inline Vector3 operator /  (Vector3 v, float f)
{
    v.x /= f;
    v.y /= f;
    v.z /= f;
    return v;
}
inline Vector3 operator *  (Vector3 v, Matrix m)
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
inline Vector3 operator *= (Vector3& v, Matrix m)
{
    Vector4 col0 = { m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0] };
    Vector4 col1 = { m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1] };
    Vector4 col2 = { m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2] };

    Vector4 row0 = { v.x, v.y, v.z, 1 };

    float x = Vector4Dot(row0,col0);
    float y = Vector4Dot(row0,col1);
    float z = Vector4Dot(row0,col2);

    v = { x, y, z };

    return v;
}

inline Vector4 operator * (Vector4 v, Matrix m)
{
    Vector4 col0 = { m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0] };
    Vector4 col1 = { m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1] };
    Vector4 col2 = { m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2] };
    Vector4 col3 = { m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3] };

    Vector4 row0 = { v.x, v.y, v.z, 1 };

    float x = Vector4Dot(row0,col0);
    float y = Vector4Dot(row0,col1);
    float z = Vector4Dot(row0,col2);
    float w = Vector4Dot(row0,col3);

    return { x, y, z, w };
}
inline Vector4 operator / (Vector4 v, float f)
{
    v.x /= f;
    v.y /= f;
    v.z /= f;
    v.w /= f;
    return v;
}

inline Matrix operator * (Matrix left, Matrix right)
{
    return MatrixMultiply(left, right);
}

inline bool InsideSphere(Vector3 point, Sphere sphere)
{
    auto diff = Vector3Subtract(point, sphere.position);
    return Vector3LengthSquared(diff) <= sphere.radius * sphere.radius;
}
inline bool RaycastFull1(Vector3 origin, Vector3 dirNorm, Sphere sphere)
{
    // TODO must be tested

    auto v1 = sphere.position - origin;
    auto v2Length = Vector3Dot(dirNorm, v1);
    auto v2 = dirNorm * v2Length;
    auto v3 = v2 - v1;
    auto v3LengthSquared = Vector3LengthSquared(v3);
    auto radiusSquared = sphere.radius * sphere.radius;

    // no intersection
    if (v3LengthSquared > radiusSquared) return false;

    auto offset = MathSqrt(radiusSquared - v3LengthSquared);

    auto dist1 = v2Length - offset;
    auto dist2 = v2Length + offset;

    auto point1 = origin + dirNorm * dist1;
    auto point2 = origin + dirNorm * dist2;

    auto normal1 = point1 - sphere.position;
    auto normal2 = point2 - sphere.position;

    normal1 = Vector3Normalize(normal1);
    normal2 = Vector3Normalize(normal2);

    return true;
}
inline bool RaycastFull2(Vector3 origin, Vector3 dirNorm, Sphere sphere)
{
    // TODO must be tested

    auto diff = origin - sphere.position;
    auto b = Vector3Dot(dirNorm, diff) * 2;
    auto c = Vector3LengthSquared(diff) - (sphere.radius * sphere.radius);
    auto deltaSquared = b * b - 4 * c;

    // no intersection
    if (deltaSquared < 0) return false;

    auto delta = MathSqrt(deltaSquared);

    auto dist1 = (-b - delta) / 2;
    auto dist2 = (-b + delta) / 2;

    auto point1 = origin + dirNorm * dist1;
    auto point2 = origin + dirNorm * dist2;

    auto normal1 = point1 - sphere.position;
    auto normal2 = point2 - sphere.position;

    normal1 = Vector3Normalize(normal1);
    normal2 = Vector3Normalize(normal2);

    return true;
}
inline bool Raycast(Vector3 origin, Vector3 dirNorm, Sphere sphere, float* outDistance, Vector3* outPoint, Vector3* outNormal)
{
    // TODO must be tested
    // Assuming the origin is outside the sphere

    auto v1 = sphere.position - origin;
    auto v2Length = Vector3Dot(dirNorm, v1);

    // sphere is behind
    if (v2Length < 0) return false;

    auto v2 = dirNorm * v2Length;
    auto v3 = v2 - v1;
    auto v3LengthSquared = Vector3LengthSquared(v3);
    auto radiusSquared = sphere.radius * sphere.radius;

    // no intersection
    if (v3LengthSquared > radiusSquared) return false;

    auto offset = MathSqrt(radiusSquared - v3LengthSquared);
    auto dist = v2Length - offset;
    auto point = origin + dirNorm * dist;
    auto normal = point - sphere.position;
    normal = Vector3Normalize(normal);

    *outDistance = dist;
    *outPoint = point;
    *outNormal = normal;

    return true;
}
inline bool LineSegmentIntersection(Vector3 start, Vector3 end, Sphere sphere)
{
    // TODO must be tested

    auto origin = start;
    auto dir = end - start;
    auto dirNorm = Vector3Normalize(dir);

    auto v1 = sphere.position - origin;
    auto v2Length = Vector3Dot(dirNorm, v1);
    auto v2 = dirNorm * v2Length;
    auto v3 = v2 - v1;
    auto v3LengthSquared = Vector3LengthSquared(v3);
    auto radiusSquared = sphere.radius * sphere.radius;

    // no intersection
    if (v3LengthSquared > radiusSquared) return false;

    auto offset = MathSqrt(radiusSquared - v3LengthSquared);
    auto dist1 = v2Length - offset;
    auto dist2 = v2Length + offset;
    auto maxDist = MathMax(dist1, dist2);

    // all points behind
    if (maxDist < 0) return false;

    // segment is short
    if (Vector3LengthSquared(dir) < maxDist * maxDist) return false;

    return true;
}

inline Pose GetLocalPose(Pose parentWorld, Pose childWorld)
{
    childWorld.rotation -= parentWorld.rotation;
    childWorld.position -= parentWorld.position;
    childWorld.position = Vector3RotateZ(childWorld.position, -parentWorld.rotation.z);
    childWorld.position = Vector3RotateY(childWorld.position, -parentWorld.rotation.y);
    childWorld.position = Vector3RotateX(childWorld.position, -parentWorld.rotation.x);
    return childWorld;
}
inline Pose GetWorldPose(Pose parentWorld, Pose childLocal)
{
    childLocal.position = Vector3RotateX(childLocal.position, parentWorld.rotation.x);
    childLocal.position = Vector3RotateY(childLocal.position, parentWorld.rotation.y);
    childLocal.position = Vector3RotateZ(childLocal.position, parentWorld.rotation.z);
    childLocal.position += parentWorld.position;
    childLocal.rotation += parentWorld.rotation;
    return childLocal;
}
