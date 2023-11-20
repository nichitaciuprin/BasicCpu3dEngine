#pragma once

// http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html

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

    Bitmap(int widthNew, int heightNew)
    {
        pixels = vector<uint32_t>();
        Resize(widthNew, heightNew);
    }

    void Resize(int widthNew, int heightNew)
    {
        if (widthNew < 1) throw exception("Bitmap widthNew < 1");
        if (heightNew < 1) throw exception("Bitmap heightNew < 1");
        width = widthNew;
        height = heightNew;
        auto pixelCount = width * height;
        if (pixels.size() < pixelCount)
            pixels.resize(pixelCount);
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
        fill(pixels.begin(), pixels.end(), pixel);
    }
    void SetPixel(int x, int y, Pixel pixel)
    {
        // TODO remove guard
        // if (x > width - 1) return;
        // if (y > height - 1) return;

        auto i = x + y * width;
        pixels[i] = pixel;
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
        Vector2Int p0 = { (int)v0.x, (int)v0.y };
        Vector2Int p1 = { (int)v1.x, (int)v1.y };
        DrawLine3(p0, p1, pixel);
    }
    void DrawLine3(Vector2Int p0, Vector2Int p1, Pixel pixel)
    {
        // int dx = abs(p1.x - p0.x);
        // int dy = abs(p1.y - p0.y);

        // int sx = p0.x < p1.x ? 1 : -1;
        // int sy = p0.y < p1.y ? 1 : -1;

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
            DrawLine2(v1, v2, WHITE);
            DrawLine2(v3, v4, WHITE);
            DrawLine2(v5, v0, WHITE);
        }

        // all in back
        // all in front
        // 2 lines cliped
        // 2 lines cliped + 1 line drop

        // ClipLineByZ3()
    }
    void DrawTriangle2(Vector3 p0, Vector3 p1, Vector3 p2, Pixel pixel)
    {
        ToScreenSpace(p0);
        ToScreenSpace(p1);
        ToScreenSpace(p2);
        Vector2Int v0 = { (int)p0.x, (int)p0.y };
        Vector2Int v1 = { (int)p1.x, (int)p1.y };
        Vector2Int v2 = { (int)p2.x, (int)p2.y };
        DrawTriangle3(v0, v1, v2, pixel);
    }
    void DrawTriangle3(Vector2Int p0, Vector2Int p1, Vector2Int p2, Pixel pixel)
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

    void ProjectLine(Vector3& v0, Vector3& v1, int& outCode)
    {
        float nearZ = 0.1f;
        v0.z -= nearZ;
        v1.z -= nearZ;
        ClipLineByZ4(v0, v1, outCode);
        if (outCode == 0) return;
        v0.z += nearZ;
        v1.z += nearZ;
        if (v0.z != 0) v0 /= v0.z;
        if (v1.z != 0) v1 /= v1.z;
    }

    inline void DrawHorizontalLine(int y, int xLeft, int xRight, Pixel pixel)
    {
        int count = xRight - xLeft + 1;
        for (int i = 0; i < count; i++)
            SetPixel(xLeft + i, y, pixel);
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
        int size = 20;
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
