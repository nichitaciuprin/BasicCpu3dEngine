// http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html

#pragma once

typedef uint32_t Pixel;

const Pixel BLACK      = 0x00000000;
const Pixel WHITE      = 0xFFFFFFFF;

const Pixel RED        = 0x00FF0000;
const Pixel GREEN      = 0x0000FF00;
const Pixel BLUE       = 0x000000FF;

const Pixel YELLOW     = 0x00FFFF00;
const Pixel MAGENTA    = 0x00FF00FF;
const Pixel CYAN       = 0x0000FFFF;

const Pixel ORANGE     = 0x00FF8000;
const Pixel PINK       = 0x00FF0080;
const Pixel LIME       = 0x0080FF00;
const Pixel GREENCOLD  = 0x0000FF80;
const Pixel VIOLET     = 0x008000FF;
const Pixel LIGHTBLUE  = 0x000080FF;

class Bitmap
{
public:
    vector<uint32_t> pixels;
    vector<float> zbuffer;

    Bitmap(int widthNew, int heightNew)
    {
        if (widthNew < 1) throw exception("Bitmap widthNew < 1");
        if (heightNew < 1) throw exception("Bitmap heightNew < 1");

        width = widthNew;
        height = heightNew;

        auto size = width * height;

        pixels = vector<uint32_t>(size, 0);
        zbuffer = vector<float>(size, 100000000.0f); // TODO
    }

    int Width() const
    {
        return width;
    }
    int Height() const
    {
        return height;
    }

    void Fill(Pixel pixel)
    {
        // TODO Move to BeginDraw()
        fill(zbuffer.begin(), zbuffer.end(), 100000000.0f);
        fill(pixels.begin(), pixels.end(), pixel);
    }

    void DrawLine1(Vector3 v0, Vector3 v1, Pixel pixel)
    {
        int outCode; ProjectLine(v0, v1, outCode);
        if (outCode == 0) return;
        DrawLine2(v0, v1, pixel);
    }
    void DrawLine2(Vector3 v0, Vector3 v1, Pixel pixel)
    {
        if (!ClipLine(v0.x, v0.y, v1.x, v1.y)) return;
        ToScreenSpace(v0);
        ToScreenSpace(v1);
        DrawLine3(v0, v1, pixel);
    }
    void DrawLine3(Vector3 v0, Vector3 v1, Pixel pixel)
    {
        Vector2Int p0 = { (int)v0.x, (int)v0.y };
        Vector2Int p1 = { (int)v1.x, (int)v1.y };

        int dx, sx;
        if (p0.x < p1.x) { dx = p1.x - p0.x; sx =  1; }
        else             { dx = p0.x - p1.x; sx = -1; }

        int dy, sy;
        if (p0.y < p1.y) { dy = p1.y - p0.y; sy =  1; }
        else             { dy = p0.y - p1.y; sy = -1; }

        // int max, min, axis1, axis2, val1, val2;
        // if (dx > dy) { max = dx; min = dy; axis1 = p0.y; axis2 = p0.x; val1 = sy; val2 = sx; }
        // else         { max = dy; min = dx; axis1 = p0.x; axis2 = p0.y; val1 = sx; val2 = sy; }

        // int err = max / 2 - min;

        // float diff = v1.z - v0.z;
        // float offset = diff / max;
        // float z = v0.z;

        // for (int i = 0; i < max / 2; i++)
        // {
        //     SetPixel2(axis2, axis1, z, pixel);
        //     if (err < 0) { err += max; axis1 += val1; }
        //                  { err -= min; axis2 += val2; }
        //     z += offset;
        // }

        // SetPixel(axis2, axis1, pixel);

        #define DRAW(MAX, MIN, AXIS1, AXIS2, VAL1, VAL2) \
        int err = MAX / 2 - MIN;                         \
        float diff = v1.z - v0.z;                        \
        float offset = diff / MAX;                       \
        float z = v0.z;                                  \
        for (int i = 0; i < MAX; i++)                    \
        {                                                \
            SetPixel2(p0.x, p0.y, z, pixel);             \
            z += offset;                                 \
            if (err < 0) { err += MAX; AXIS1 += VAL1; }  \
                         { err -= MIN; AXIS2 += VAL2; }  \
        }                                                \
        SetPixel(p0.x, p0.y, pixel);                     \

        if (dx > dy) { DRAW(dx, dy, p0.y, p0.x, sy, sx); }
        else         { DRAW(dy, dx, p0.x, p0.y, sx, sy); }

        #undef DRAW
    }
    void DrawLine4(Vector2Int p0, Vector2Int p1, Pixel pixel)
    {
        int sx, dx; if (p0.x < p1.x) { sx = 1; dx = p1.x - p0.x; } else { sx = -1; dx = p0.x - p1.x; }
        int sy, dy; if (p0.y < p1.y) { sy = 1; dy = p1.y - p0.y; } else { sy = -1; dy = p0.y - p1.y; }

        #define DRAW(MAX, MIN, AXIS1, AXIS2, VAL1, VAL2)  \
        int err = MAX / 2;                                \
        for (int i = 0; i < MAX + 1; i++)                 \
        {                                                 \
            SetPixel(p0.x, p0.y, pixel);                  \
            if (err < MIN) { err += MAX; AXIS1 += VAL1; } \
                           { err -= MIN; AXIS2 += VAL2; } \
        }                                                 \

        if (dx > dy) { DRAW(dx, dy, p0.y, p0.x, sy, sx); }
        else         { DRAW(dy, dx, p0.x, p0.y, sx, sy); }

        #undef DRAW
    }

    void DrawTriangle1(Vector3 p0, Vector3 p1, Vector3 p2, Pixel pixel)
    {
        Vector3 v1 = p0; Vector3 v2 = p1;
        Vector3 v3 = p1; Vector3 v4 = p2;
        Vector3 v5 = p2; Vector3 v0 = p0;

        int outCode1; ProjectLine(v1, v2, outCode1);
        int outCode2; ProjectLine(v3, v4, outCode2);
        int outCode3; ProjectLine(v5, v0, outCode3);

        if (outCode1 == 0 && outCode2 == 0 && outCode3 == 0) return;
        if (outCode1 == 2 && outCode2 == 2 && outCode3 == 2)
        {
            if (!Vector3TriangleIsClockwise(v1, v3, v5)) return;
            DrawTriangle2(v1, v3, v5, pixel);
            // DrawLine2(v1, v2, WHITE);
            // DrawLine2(v3, v4, WHITE);
            // DrawLine2(v5, v0, WHITE);
        }

        // all in back
        // all in front
        // 2 lines cliped
        // 2 lines cliped + 1 line drop

        // ClipLineByZ3()
    }
    void DrawTriangle2(Vector3 p0, Vector3 p1, Vector3 p2, Pixel pixel)
    {
        // TODO replace with proper triangle cliping
        if (abs(p0.x) > 1) return;
        if (abs(p0.y) > 1) return;
        if (abs(p1.x) > 1) return;
        if (abs(p1.y) > 1) return;
        if (abs(p2.x) > 1) return;
        if (abs(p2.y) > 1) return;

        ToScreenSpace(p0);
        ToScreenSpace(p1);
        ToScreenSpace(p2);

        DrawTriangle3(p0, p1, p2, pixel);
    }
    void DrawTriangle3(Vector3 v0, Vector3 v1, Vector3 v2, Pixel pixel)
    {
        // p0 is top
        // p1 is middle
        // p2 is bottom
        if (v0.y > v1.y) swap(v0, v1);
        if (v1.y > v2.y) swap(v1, v2);
        if (v0.y > v1.y) swap(v0, v1);

        DrawLine3(v0, v2, RED);
        // DrawLine3(v0, v1, RED);
        // DrawLine3(v1, v2, RED);

        // DrawLineShort((int)v0.x, (int)v0.y, (int)v2.x, (int)v2.y, WHITE);
        // DrawLineShort((int)v0.x, (int)v0.y, (int)v1.x, (int)v1.y, WHITE);
        // DrawLineShort((int)v1.x, (int)v1.y, (int)v2.x, (int)v2.y, WHITE);

        return;

        Vector2Int p0 = { (int)v0.x, (int)v0.y };
        Vector2Int p1 = { (int)v1.x, (int)v1.y };
        Vector2Int p2 = { (int)v2.x, (int)v2.y };
        int dx1 = p2.x - p0.x;
        int dx2 = p1.x - p0.x;
        int dx3 = p2.x - p1.x;
        int dy1 = p2.y - p0.y;
        int dy2 = p1.y - p0.y;
        int dy3 = p2.y - p1.y;
        int dir1 = MathSign(dx1);
        int dir2 = MathSign(dx2);
        int dir3 = MathSign(dx3);
        int dx1abs = abs(dx1);
        int dx2abs = abs(dx2);
        int dx3abs = abs(dx3);
        // int max1 = MathMax(dy1, dx1abs);
        // int max2 = MathMax(dy2, dx2abs);
        // int max3 = MathMax(dy3, dx3abs);
        // int min1 = MathMin(dy1, dx1abs);
        // int min2 = MathMin(dy2, dx2abs);
        // int min3 = MathMin(dy3, dx3abs);
        int err1 = dy1 / 2 - dx1abs;
        int err2 = dy2 / 2 - dx2abs;
        int err3 = dy3 / 2 - dx3abs;
        int cross = dx1 * dy2 - dy1 * dx2;

        // TODO maybe check for null
        // float offset1 = dy1 == 0 ? 0 : (v2.z - v0.z) / dy1;
        // float offset2 = dy2 == 0 ? 0 : (v1.z - v0.z) / dy2;
        // float offset3 = dy3 == 0 ? 0 : (v2.z - v1.z) / dy3;
        float offset1 = (v2.z - v0.z) / dy1;
        float offset2 = (v1.z - v0.z) / dy2;
        float offset3 = (v2.z - v1.z) / dy3;

        int y = p0.y;

        int x1 = p0.x;
        float z1 = v0.z;

        int x2;
        float z2;
        if (dy2 > 0) { x2 = p0.x; z2 = v0.z; }
        else         { x2 = p1.x; z2 = v1.z; }

        #define DRAW(X1, X2, Z1, Z2)                       \
        for (int i = 0; i < dy2; i++)                      \
        {                                                  \
            while (err1 < 0) { err1 += dy1; x1 += dir1; }  \
            while (err2 < 0) { err2 += dy2; x2 += dir2; }  \
            DrawHorizontalLine2(y, X1, X2, Z1, Z2, pixel); \
            y++;                                           \
            err1 -= dx1abs;                                \
            err2 -= dx2abs;                                \
            z1 += offset1;                                 \
            z2 += offset2;                                 \
        }                                                  \
        for (int i = 0; i < dy3; i++)                      \
        {                                                  \
            while (err1 < 0) { err1 += dy1; x1 += dir1; }  \
            while (err3 < 0) { err3 += dy3; x2 += dir3; }  \
            DrawHorizontalLine2(y, X1, X2, Z1, Z2, pixel); \
            y++;                                           \
            err1 -= dx1abs;                                \
            err3 -= dx3abs;                                \
            z1 += offset1;                                 \
            z2 += offset3;                                 \
        }                                                  \
        DrawHorizontalLine2(y, X1, X2, Z1, Z2, pixel);     \

        if (cross < 0)
        {
            DRAW(x1, x2, z1, z2)
        }
        else
        {
            DRAW(x2, x1, z2, z1)
        }

        #undef DRAW

        // DrawLine3(v0, v2, WHITE);
        // DrawLine3(v0, v1, WHITE);
        // DrawLine3(v1, v2, WHITE);
    }
    void DrawTriangle4(Vector2Int p0, Vector2Int p1, Vector2Int p2, Pixel pixel)
    {
        // p0 is top
        // p1 is middle
        // p2 is bottom
        if (p0.y > p1.y) swap(p0, p1);
        if (p1.y > p2.y) swap(p1, p2);
        if (p0.y > p1.y) swap(p0, p1);
        int dx1 = p2.x - p0.x;
        int dx2 = p1.x - p0.x;
        int dx3 = p2.x - p1.x;
        int dy1 = p2.y - p0.y;
        int dy2 = p1.y - p0.y;
        int dy3 = p2.y - p1.y;
        int err1 = dy1 / 2;
        int err2 = dy2 / 2;
        int err3 = dy3 / 2;
        int dir1 = MathSign(dx1);
        int dir2 = MathSign(dx2);
        int dir3 = MathSign(dx3);
        int dx1abs = abs(dx1);
        int dx2abs = abs(dx2);
        int dx3abs = abs(dx3);
        int cross = dx1 * dy2 - dy1 * dx2;

        int y = p0.y;
        int x1, x2;
        if (dy2 > 0) { x1 = p0.x; x2 = p0.x; }
        else         { x1 = p0.x; x2 = p1.x; }

        #define DRAW(X1, X2)                              \
        for (int i = 0; i < dy2; i++)                     \
        {                                                 \
            DrawHorizontalLine(y, X1, X2, pixel);         \
            y++;                                          \
            err1 -= dx1abs;                               \
            err2 -= dx2abs;                               \
            while (err1 < 0) { err1 += dy1; x1 += dir1; } \
            while (err2 < 0) { err2 += dy2; x2 += dir2; } \
        }                                                 \
        for (int i = 0; i < dy3; i++)                     \
        {                                                 \
            DrawHorizontalLine(y, X1, X2, pixel);         \
            y++;                                          \
            err1 -= dx1abs;                               \
            err3 -= dx3abs;                               \
            while (err1 < 0) { err1 += dy1; x1 += dir1; } \
            while (err3 < 0) { err3 += dy3; x2 += dir3; } \
        }                                                 \
        DrawHorizontalLine(y, X1, X2, pixel);             \

        if (cross < 0)
        {
            DRAW(x1, x2)
        }
        else
        {
            DRAW(x2, x1)
        }

        #undef DRAW
    }

    void DrawPoligon(Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3, Pixel pixel)
    {
        DrawTriangle1(p0, p1, p2, pixel);
        DrawTriangle1(p2, p3, p0, pixel);
    }
    void DrawPoligon2(Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3, Pixel pixel)
    {
        DrawTriangle2(p0, p1, p2, pixel);
        DrawTriangle2(p2, p3, p0, pixel);
    }

    void ProjectLine(Vector3& v0, Vector3& v1, int& outCode)
    {
        float nearZ = 0.1f;
        v0.z -= nearZ;
        v1.z -= nearZ;
        ClipLineByZ4(v0, v1, outCode);
        if (outCode == 0) return;
        v0.z += nearZ;
        v1.z += nearZ;
        if (v0.z != 0) { v0.x /= v0.z; v0.y /= v0.z; };
        if (v1.z != 0) { v1.x /= v1.z; v1.y /= v1.z; };
    }

    inline void DrawHorizontalLine(int y, int xLeft, int xRight, Pixel pixel)
    {
        int count = xRight - xLeft + 1;
        for (int i = 0; i < count; i++)
            SetPixel(xLeft + i, y, pixel);
    }
    inline void DrawHorizontalLine2(int y, int xLeft, int xRight, float zLeft, float zRight, Pixel pixel)
    {
        int count = xRight - xLeft;
        float diff = zRight - zLeft;
        float offset = diff / count;

        for (int i = 0; i < count + 1; i++)
        {
            auto x = xLeft + i;
            // SetPixel2(x, y, zLeft, pixel);
            SetPixel(x, y, pixel);
            zLeft += offset;
        }
    }

    void SetPixel(int x, int y, Pixel pixel)
    {
        // TODO remove guard
        // if (x > width - 1) return;
        // if (y > height - 1) return;
        auto i = x + y * width;
        pixels[i] = pixel;
    }
    void SetPixel2(int x, int y, float z, Pixel pixel)
    {
        // TODO remove guard
        // if (x > width - 1) return;
        // if (y > height - 1) return;

        auto i = x + y * width;

        // TODO maybe drop equal
        if (zbuffer[i] >= z)
        {
            zbuffer[i] = z;
            pixels[i] = pixel;
        }
    }

    void DrawCube1(Matrix modelView)
    {
        float h = 0.5f;
        Vector3 vertices[] =
        {
            Vector3{-h,-h,-h},
            Vector3{-h,-h, h},
            Vector3{-h, h,-h},
            Vector3{-h, h, h},
            Vector3{ h,-h,-h},
            Vector3{ h,-h, h},
            Vector3{ h, h,-h},
            Vector3{ h, h, h}
        };

        int indices[12][2] =
        {
            0,1,
            1,5,
            5,4,
            4,0,
            2,3,
            3,7,
            7,6,
            6,2,
            0,2,
            1,3,
            5,7,
            4,6
        };

        for (size_t i = 0; i < 8; i++)
            vertices[i] *= modelView;

        for (size_t i = 0; i < 12; i++)
        {
            auto i0 = indices[i][0];
            auto i1 = indices[i][1];
            DrawLine1(vertices[i0], vertices[i1], RED);
        }
    }
    void DrawCube2(Matrix modelView)
    {
        float h = 0.5f;

        Vector3 vertexData[] =
        {
            Vector3{-h,-h,-h},
            Vector3{-h,-h, h},
            Vector3{-h, h,-h},
            Vector3{-h, h, h},
            Vector3{ h,-h,-h},
            Vector3{ h,-h, h},
            Vector3{ h, h,-h},
            Vector3{ h, h, h}
        };

        int indexData[12][3] =
        {
            0, 2, 6,
            6, 4, 0,
            4, 6, 7,
            7, 5, 4,
            5, 7, 3,
            3, 1, 5,
            1, 3, 2,
            2, 0, 1,
            2, 3, 7,
            7, 6, 2,
            1, 0, 4,
            4, 5, 1,
        };

        for (int i = 0; i < 8; i++)
            vertexData[i] *= modelView;

        for (int i = 0; i < 12; i++)
        {
            auto i0 = indexData[i][0];
            auto i1 = indexData[i][1];
            auto i2 = indexData[i][2];
            DrawTriangle1(vertexData[i0], vertexData[i1], vertexData[i2], RED);
        }

        {
            auto i0 = indexData[0][0];
            auto i1 = indexData[0][1];
            auto i2 = indexData[0][2];
            DrawTriangle1(vertexData[i0], vertexData[i1], vertexData[i2], GREEN);
        }
        {
            auto i0 = indexData[1][0];
            auto i1 = indexData[1][1];
            auto i2 = indexData[1][2];
            DrawTriangle1(vertexData[i0], vertexData[i1], vertexData[i2], BLUE);
        }
        {
            auto i0 = indexData[3][0];
            auto i1 = indexData[3][1];
            auto i2 = indexData[3][2];
            DrawTriangle1(vertexData[i0], vertexData[i1], vertexData[i2], GREEN);
        }
        {
            auto i0 = indexData[4][0];
            auto i1 = indexData[4][1];
            auto i2 = indexData[4][2];
            DrawTriangle1(vertexData[i0], vertexData[i1], vertexData[i2], BLUE);
        }
    }
    void DrawCube3(Matrix modelView)
    {
        float h = 0.5f;

        Vector3 vertexData[] =
        {
            Vector3{-h,-h,-h},
            Vector3{-h,-h, h},
            Vector3{-h, h,-h},
            Vector3{-h, h, h},
            Vector3{ h,-h,-h},
            Vector3{ h,-h, h},
            Vector3{ h, h,-h},
            Vector3{ h, h, h}
        };

        int indexData[12][3] =
        {
            0, 2, 6,
            6, 4, 0,
            4, 6, 7,
            7, 5, 4,
            5, 7, 3,
            3, 1, 5,
            1, 3, 2,
            2, 0, 1,
            2, 3, 7,
            7, 6, 2,
            1, 0, 4,
            4, 5, 1,
        };

        for (int i = 0; i < 8; i++)
            vertexData[i] *= modelView;

        for (int i = 0; i < 12; i++)
        {
            auto i0 = indexData[i][0];
            auto i1 = indexData[i][1];
            auto i2 = indexData[i][2];

            Pixel           pixel = RED;
            if (i % 3 == 1) pixel = GREEN;
            if (i % 3 == 2) pixel = BLUE;

            DrawTriangle1(vertexData[i0], vertexData[i1], vertexData[i2], pixel);
        }
    }
    void DrawCube4(Matrix modelView)
    {
        float h = 0.5f;

        Vector3 vertexData[] =
        {
            Vector3{-h,-h,-h},
            Vector3{-h,-h, h},
            Vector3{-h, h,-h},
            Vector3{-h, h, h},
            Vector3{ h,-h,-h},
            Vector3{ h,-h, h},
            Vector3{ h, h,-h},
            Vector3{ h, h, h}
        };

        int indexData[6][4] =
        {
            2, 6, 4, 0,
            6, 7, 5, 4,
            7, 3, 1, 5,
            3, 2, 0, 1,
            1, 0, 4, 5,
            3, 7, 6, 2,
        };

        for (int i = 0; i < 8; i++)
            vertexData[i] *= modelView;

        #define DRAW(INDEX, COLOR)                     \
        {                                              \
            auto p0 = vertexData[indexData[INDEX][0]]; \
            auto p1 = vertexData[indexData[INDEX][1]]; \
            auto p2 = vertexData[indexData[INDEX][2]]; \
            auto p3 = vertexData[indexData[INDEX][3]]; \
            DrawPoligon(p0, p1, p2, p3, COLOR);        \
        }                                              \

        // DRAW(0, GREEN)
        // DRAW(1, RED)
        // DRAW(2, GREEN)
        // DRAW(3, RED)
        // DRAW(4, GREEN)
        // DRAW(5, GREEN)

        DRAW(0, CYAN)
        DRAW(1, GREEN)
        DRAW(2, BLUE)
        DRAW(3, YELLOW)
        DRAW(4, MAGENTA)
        DRAW(5, RED)

        #undef DRAW
    }

    void ToScreenSpace(Vector3& point)
    {
        point.y = -point.y;
        point.x += 1.0f;
        point.y += 1.0f;
        point.x /= 2;
        point.y /= 2;
        point.x = (width - 1) * point.x;
        point.y = (height - 1) * point.y;
    }

    void DrawBorder(Pixel pixel)
    {
        int x = width - 1;
        int y = height - 1;
        for (int i = 0; i < width;  i++) SetPixel(i, 0, pixel);
        for (int i = 0; i < width;  i++) SetPixel(i, y, pixel);
        for (int i = 0; i < height; i++) SetPixel(0, i, pixel);
        for (int i = 0; i < height; i++) SetPixel(x, i, pixel);
    }
    void DrawCross(Pixel pixel)
    {
        int centerX = width / 2;
        int centerY = height / 2;
        for (int i = 0; i < width;  i++) SetPixel(i, centerY, pixel);
        for (int i = 0; i < height; i++) SetPixel(centerX, i, pixel);
    }
    void DrawSquare(Pixel pixel)
    {
        // TODO
        int size = 4;
        for (int x = 0; x < MathClamp(size, 0, width - 1); x++)
        for (int y = 0; y < MathClamp(size, 0, height - 1); y++)
            SetPixel(x,y,pixel);
    }

    void ScanEffectRandom(size_t iteration)
    {
        size_t i1 = iteration;
        size_t i2 = Subgen::StaticNext();
        size_t pixelCount = width * height;
        i1 %= pixelCount;
        i2 %= pixelCount;
        pixels[i1] = Subgen::StaticNext();
        pixels[i2] = 0;
    }
    void DrawLineShort(int x0, int y0, int x1, int y1, Pixel pixel)
    {
        int dx = abs(x1 - x0);
        int dy = abs(y1 - y0);
        int sx = x0 < x1 ? 1 : -1;
        int sy = y0 < y1 ? 1 : -1;

        int err = (dx > dy ? dx : -dy) / 2;
        int err2;

        while (true)
        {
            SetPixel(x0, y0, pixel);
            if (x0 == x1 && y0 == y1) break;
            err2 = err;
            if (err2 > -dx) { err -= dy; x0 += sx; }
            if (err2 <  dy) { err += dx; y0 += sy; }
        }
    }

private:
    int width = 0;
    int height = 0;
};
