#pragma once

// http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html

typedef uint32_t Pixel;

const Pixel BLACK = 0x00000000;
const Pixel WHITE = 0xFFFFFFFF;
const Pixel RED   = 0x00FF0000;
const Pixel GREEN = 0x0000FF00;
const Pixel BLUE  = 0x000000FF;

struct Vector3Int
{
    int x, y, z;
};

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

    void DrawTriangle(Vector3 p0, Vector3 p1, Vector3 p2, Pixel pixel)
    {
        Vector3 v1 = p0; Vector3 v2 = p1;
        Vector3 v3 = p1; Vector3 v4 = p2;
        Vector3 v5 = p2; Vector3 v0 = p0;

        bool include0; ProjectLine(v1, v2, include0);
        bool include1; ProjectLine(v3, v4, include1);
        bool include2; ProjectLine(v5, v0, include2);

        bool includeAll = include0 && include1 && include2;

        if (!includeAll) return;

        if (includeAll)
        {
            if (!Vector3TriangleIsClockwise(v1, v3, v5)) return;
            DrawLineProjected(v1, v2, pixel);
            DrawLineProjected(v3, v4, pixel);
            DrawLineProjected(v5, v0, pixel);
            return;
        }
        // if (!include0)
        // {
        //     if (!Vector3TriangleIsClockwise(v3, v4, v0)) return;
        //     DrawLineProjected(v3, v4, pixel);
        //     DrawLineProjected(v5, v0, pixel);
        //     return;
        // }
        // if (!include1)
        // {
        //     if (!Vector3TriangleIsClockwise(v1, v2, v0)) return;
        //     DrawLineProjected(v1, v2, pixel);
        //     DrawLineProjected(v5, v0, pixel);
        //     return;
        // }
        // if (!include2)
        // {
        //     if (!Vector3TriangleIsClockwise(v1, v2, v4)) return;
        //     DrawLineProjected(v1, v2, pixel);
        //     DrawLineProjected(v3, v4, pixel);
        //     return;
        // }
    }
    void DrawLine(Vector3 v0, Vector3 v1, Pixel pixel)
    {
        float nearZ = 0.1f;
        v0.z -= nearZ;
        v1.z -= nearZ;
        if (!ClipLineByZ(v0, v1)) return;
        v0.z += nearZ;
        v1.z += nearZ;
        if (v0.z != 0) v0 /= v0.z;
        if (v1.z != 0) v1 /= v1.z;
        DrawLineProjected(v0, v1, pixel);
    }
    void ProjectLine(Vector3& v0, Vector3& v1, bool& include)
    {
        float nearZ = 0.1f;
        v0.z -= nearZ;
        v1.z -= nearZ;
        include = ClipLineByZ(v0, v1);
        if (!include) return;
        v0.z += nearZ;
        v1.z += nearZ;
        if (v0.z != 0) v0 /= v0.z;
        if (v1.z != 0) v1 /= v1.z;
    }
    void DrawLineProjected(Vector3 v0, Vector3 v1, Pixel pixel)
    {
        if (!ClipLine(v0.x, v0.y, v1.x, v1.y)) return;
        int outX0, outY0;
        int outX1, outY1;
        ToScreenSpace(v0, &outX0, &outY0);
        ToScreenSpace(v1, &outX1, &outY1);
        DrawLine(outX0, outY0, outX1, outY1, pixel);
    }

    void DrawLine(int x0, int y0, int x1, int y1, Pixel pixel)
    {
        int dx = abs(x1 - x0);
        int dy = abs(y1 - y0);

        int sx = x0 < x1 ? 1 : -1;
        int sy = y0 < y1 ? 1 : -1;

        #define DRAW(MAX, MIN, AXIS1, AXIS2, VAL1, VAL2)  \
        int err = MAX / 2;                                \
        for (int i = 0; i < MAX; i++)                     \
        {                                                 \
            SetPixel(x0, y0, pixel);                      \
            if (err < MIN) { err += MAX; AXIS1 += VAL1; } \
                           { err -= MIN; AXIS2 += VAL2; } \
        }                                                 \

        if (dx > dy) { DRAW(dx, dy, y0, x0, sy, sx); }
        else         { DRAW(dy, dx, x0, y0, sx, sy); }

        #undef DRAW
    }

    void ScreenSpaceDrawTriangle(Vector3 p0, Vector3 p1, Vector3 p2, Pixel pixel)
    {
        if (p2.y > p1.y) swap(p2, p1);
        if (p1.y > p0.y) swap(p1, p0);
        if (p2.y > p1.y) swap(p2, p1);

        DrawTriangle2(p2.x, p2.y, p1.x, p1.y, p0.x, p0.y, pixel);
    }

    void DrawTriangle2(int xTop, int yTop, int xMiddle, int yMiddle, int xBottom, int yBottom, Pixel pixel)
    {
        int dir1 = MathSign(xBottom - xTop);
        int dir2 = MathSign(xMiddle - xTop);
        int dx1 = abs(xBottom - xTop);
        int dx2 = abs(xMiddle - xTop);
        int dx3 = abs(xBottom - xMiddle);
        int dy1 = yBottom - yTop;
        int dy2 = yMiddle - yTop;
        int dy3 = yBottom - yMiddle;
        int err1 = dy1 / 2;
        int err2 = dy2 / 2;
        int err3 = dy3 / 2;
        int y = yTop;
        int x1 = xTop;
        int x2 = xTop;

        // int* lx; int* rx;
        // if (xTop < xBottom) { lx = &x1; rx = &x2; }
        // else                { lx = &x2; rx = &x1; }

        #define DRAW(MAX, MIN, AXIS1, AXIS2, VAL1, VAL2)  \
        int err = MAX / 2;                                \
        for (int i = 0; i < MAX; i++)                     \
        {                                                 \
            SetPixel(x0, y0, pixel);                      \
            if (err < MIN) { err += MAX; AXIS1 += VAL1; } \
                           { err -= MIN; AXIS2 += VAL2; } \
        }                                                 \

        if (dx > dy) { DRAW(dx, dy, y0, x0, sy, sx); }
        else         { DRAW(dy, dx, x0, y0, sx, sy); }

        #undef DRAW

        for (int i = 0; i < dy2; i++)
        {
            DrawHorizontalLine(y, *lx, *rx, pixel);
            y++;
            err1 -= dx1;
            err2 -= dx2;
            while (err1 < 0) { err1 += dy1; *rx += dir1; }
            while (err2 < 0) { err2 += dy2; *lx += dir2; }
        }

        dir2 = -dir2;

        for (int i = 0; i < dy3; i++)
        {
            DrawHorizontalLine(y, *lx, *rx, pixel);
            y++;
            err1 -= dx1;
            err3 -= dx3;
            while (err1 < 0) { err1 += dy1; *rx += dir1; }
            while (err3 < 0) { err3 += dy3; *lx += dir2; }
        }
    }

    int FindPointXMiddle2(int x0, int y0, int x1, int y1, int x2, int y2)
    {
        UNREFERENCED_PARAMETER(x1);
        return x0 + ((y1 - y0) / (y2 - y0)) * (x2 - x0);
    }

    // void DrawTriangleTop(int x0, int y0, int x1, int y1, int x2, int y2, Pixel pixel)
    // {
    //     int dx = abs(x1 - x0);
    //     int dy = abs(y1 - y0);

    //     int sx = x0 < x1 ? 1 : -1;
    //     int sy = y0 < y1 ? 1 : -1;

    //     if (dx > dy)
    //     {
    //         int err = dx / 2;
    //         for (int i = 0; i < dx; i++)
    //         {
    //             SetPixel(x0, y0, pixel);
    //             if (err < dy) { err += dx; y0 += sy; }
    //                           { err -= dy; x0 += sx; }
    //         }
    //     }
    //     else
    //     {
    //         int err = dy / 2;
    //         for (int i = 0; i < dy; i++)
    //         {
    //             SetPixel(x0, y0, pixel);
    //             if (err < dx) { err += dy; x0 += sx; }
    //                           { err -= dx; y0 += sy; }
    //         }
    //     }
    // }

    void DrawTriangleTop(int x0, int y0, int x1, int y1, int x2, int y2, Pixel pixel)
    {
        //      0
        //      /\
        //     /  \
        //    /    \
        //  1 ------ 2

        int dy = y1 - y0;

        int x1Diff = x0 - x1;
        int x2Diff = x2 - x0;

        int x1Length = abs(x1Diff);
        int x2Length = abs(x2Diff);

        int x1Dir = x1Diff > 0 ? -1 :  1;
        int x2Dir = x2Diff > 0 ?  1 : -1;

        int x1Err = dy / 2;
        int x2Err = dy / 2;

        x1 = x0;
        x2 = x0;

        for (int i = 0; i < dy; i++)
        {
            DrawHorizontalLine(y0, x1, x2, pixel);

            y0++;

            x1Err -= x1Length;
            x2Err -= x2Length;

            while (x1Err < 0) { x1Err += dy; x1 += x1Dir; }
            while (x2Err < 0) { x2Err += dy; x2 += x2Dir; }
        }
    }

    void DrawTriangleBottom(int x0, int y0, int x1, int y1, int x2, int y2, Pixel pixel)
    {
        //  1 ------ 2
        //    \    /
        //     \  /
        //      \/
        //      0

        int dy = y0 - y1;

        int x1Diff = x0 - x1;
        int x2Diff = x2 - x0;

        int x1Length = abs(x1Diff);
        int x2Length = abs(x2Diff);

        int x1Dir = x1Diff > 0 ? -1 :  1;
        int x2Dir = x2Diff > 0 ?  1 : -1;

        int x1Err = dy / 2;
        int x2Err = dy / 2;

        x1 = x0;
        x2 = x0;

        for (int i = 0; i < dy; i++)
        {
            DrawHorizontalLine(y0, x1, x2, pixel);

            y0--;

            x1Err -= x1Length;
            x2Err -= x2Length;

            while (x1Err <= 0) { x1Err += dy; x1 += x1Dir; }
            while (x2Err <= 0) { x2Err += dy; x2 += x2Dir; }
        }
    }

    inline void DrawHorizontalLine(int y, int xLeft, int xRight, Pixel pixel)
    {
        int count = xRight - xLeft + 1;
        for (int i = 0; i < count; i++)
            SetPixel(xLeft + i, y, pixel);
    }

    void DrawCube(Matrix modelView)
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
            DrawLine(vertices[i0], vertices[i1], RED);
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
            DrawTriangle(vertexData[i0], vertexData[i1], vertexData[i2], RED);
        }

        // for (int i = 1; i < 12; i++)
        // {
        //     auto i0 = indexData[i][0];
        //     auto i1 = indexData[i][1];
        //     auto i2 = indexData[i][2];
        //     DrawTriangle(vertexData[i0], vertexData[i1], vertexData[i2], BLUE);
        // }

        // {
        //     auto i0 = indexData[0][0];
        //     auto i1 = indexData[0][1];
        //     auto i2 = indexData[0][2];
        //     DrawTriangle(vertexData[i0], vertexData[i1], vertexData[i2], RED);
        // }
    }
    void ToScreenSpace(Vector3 point, int* outX, int* outY)
    {
        point.y = -point.y;
        point.x += 1.0f;
        point.y += 1.0f;
        point.x /= 2;
        point.y /= 2;
        *outX = (int)((width - 1) * point.x);
        *outY = (int)((height - 1) * point.y);
    }

    void DrawBorder(Pixel pixel)
    {
        int x = width - 1;
        int y = height - 1;
        DrawLine(0, 0, 0, y, pixel);
        DrawLine(0, 0, x, 0, pixel);
        DrawLine(x, y, x, 0, pixel);
        DrawLine(x, y, 0, y, pixel);
    }
    void DrawX(Pixel pixel)
    {
        int x = width - 1;
        int y = height - 1;
        DrawLine(0, 0, x, y, pixel);
        DrawLine(x, 0, 0, y, pixel);
    }
    void DrawCross(Pixel pixel)
    {
        int centerX = width / 2;
        int centerY = height / 2;
        int x = width - 1;
        int y = height - 1;
        DrawLine(centerX, 0, centerX, y, pixel);
        DrawLine(0, centerY, x, centerY, pixel);
    }
    void DrawSquare(Pixel pixel)
    {
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
