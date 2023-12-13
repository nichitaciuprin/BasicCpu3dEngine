// http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html
// https://developer.nvidia.com/gpugems/gpugems2/part-v-image-oriented-computing/chapter-42-conservative-rasterization

#pragma once

typedef uint32_t Pixel;

struct Color
{
    uint8_t a, r, g, b;
};

const Pixel BLACK      = 0x00000000;
const Pixel WHITE      = 0x00FFFFFF;
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
    float nearZ = 0.2f;

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
        int outCode;
        ClipLine(v0.x, v0.y, v1.x, v1.y, outCode);
        if (outCode == 0) return;
        ToScreenSpace(v0);
        ToScreenSpace(v1);
        DrawLine3(v0, v1, pixel);
    }
    void DrawLine3(Vector3 v0, Vector3 v1, Pixel pixel)
    {
        int x0 = (int)v0.x;
        int y0 = (int)v0.y;

        int x1 = (int)v1.x;
        int y1 = (int)v1.y;

        int dx, sx;
        if (x0 < x1) { dx = x1 - x0; sx =  1; }
        else         { dx = x0 - x1; sx = -1; }

        int dy, sy;
        if (y0 < y1) { dy = y1 - y0; sy =  1; }
        else         { dy = y0 - y1; sy = -1; }

        int max; int* axis1; int val1;
        int min; int* axis2; int val2;
        if (dx > dy) { max = dx; axis1 = &y0; val1 = sy; min = dy; axis2 = &x0; val2 = sx; }
        else         { max = dy; axis1 = &x0; val1 = sx; min = dx; axis2 = &y0; val2 = sy; }

        int err = max / 2 - min;

        float offset = (v1.z - v0.z) / max;
        float z = v0.z;

        for (int i = 0; i < max; i++)
        {
            SetPixel2(x0, y0, z, pixel);
            if (err < 0) { err += max; (*axis1) += val1; }
                         { err -= min; (*axis2) += val2; }
            z += offset;
        }
        SetPixel2(x0, y0, z, pixel);
    }

    void DrawTriangle0(Vector3& p0, Vector3& p1, Vector3& p2, const Pixel& pixel, Matrix& view)
    {
        p0 *= view;
        p1 *= view;
        p2 *= view;
        DrawTriangle1(p0, p1, p2, pixel);
    }
    void DrawTriangle1(Vector3 p0, Vector3 p1, Vector3 p2, Pixel pixel)
    {
        p0.z -= nearZ;
        p1.z -= nearZ;
        p2.z -= nearZ;

        int state = 0;

        if (p2.z < 0) state += 1;
        if (p1.z < 0) state += 2;
        if (p0.z < 0) state += 4;

        Vector3 v0, v1, v2;

        switch (state)
        {
            /* 000 */ case 0: {                            goto T0; };
            /* 001 */ case 1: { v0 = p2; v1 = p0; v2 = p1; goto T1; };
            /* 010 */ case 2: { v0 = p1; v1 = p2; v2 = p0; goto T1; };
            /* 100 */ case 4: { v0 = p0; v1 = p1; v2 = p2; goto T1; };
            /* 011 */ case 3: { v0 = p0; v1 = p1; v2 = p2; goto T2; };
            /* 101 */ case 5: { v0 = p1; v1 = p0; v2 = p2; goto T2; };
            /* 110 */ case 6: { v0 = p2; v1 = p0; v2 = p1; goto T2; };
            /* 111 */ case 7: { return; };
            default: abort();
        }

        T0:
        {
            p0.z += nearZ;
            p1.z += nearZ;
            p2.z += nearZ;
            if (p0.z != 0) { p0.x /= p0.z; p0.y /= p0.z; };
            if (p1.z != 0) { p1.x /= p1.z; p1.y /= p1.z; };
            if (p2.z != 0) { p2.x /= p2.z; p2.y /= p2.z; };
            DrawTriangle2(p0, p1, p2, pixel);
            return;
        }

        T1:
        {
            Vector3 b1 = v0;
            Vector3 b2 = v0;
            b1 += (b1 - v1) * b1.z / (v1.z - b1.z);
            b2 += (b2 - v2) * b2.z / (v2.z - b2.z);
            v0.z += nearZ;
            v1.z += nearZ;
            v2.z += nearZ;
            if (v1.z != 0) { v1.x /= v1.z; v1.y /= v1.z; };
            if (v2.z != 0) { v2.x /= v2.z; v2.y /= v2.z; };
            DrawTriangle2(b1, v1, b2, pixel);
            DrawTriangle2(b2, v1, v2, pixel);
            return;
        }

        T2:
        {
            v1 += (v1 - v0) * v1.z / (v0.z - v1.z);
            v2 += (v2 - v0) * v2.z / (v0.z - v2.z);
            v0.z += nearZ;
            v1.z += nearZ;
            v2.z += nearZ;
            if (v0.z != 0) { v0.x /= v0.z; v0.y /= v0.z; };
            DrawTriangle2(v0, v1, v2, pixel);
            return;
        }
    }
    void DrawTriangle2(Vector3 p0, Vector3 p1, Vector3 p2, Pixel pixel)
    {
        if (!Vector3TriangleIsClockwise(p0, p1, p2)) return;

        int state = 0;

        if (abs(p2.x) > 1 || abs(p2.y) > 1) state += 1;
        if (abs(p1.x) > 1 || abs(p1.y) > 1) state += 2;
        if (abs(p0.x) > 1 || abs(p0.y) > 1) state += 4;

        Vector3 v0, v1, v2;

        switch (state)
        {
            /* 000 */ case 0: {                            goto T0; };
            /* 001 */ case 1: { v0 = p2; v1 = p0; v2 = p1; goto T1; };
            /* 010 */ case 2: { v0 = p1; v1 = p2; v2 = p0; goto T1; };
            /* 100 */ case 4: { v0 = p0; v1 = p1; v2 = p2; goto T1; };
            /* 011 */ case 3: { v0 = p0; v1 = p1; v2 = p2; goto T2; };
            /* 101 */ case 5: { v0 = p1; v1 = p0; v2 = p2; goto T2; };
            /* 110 */ case 6: { v0 = p2; v1 = p0; v2 = p1; goto T2; };
            /* 111 */ case 7: { return; };
            default: abort();
        }

        T0:
        {
            ToScreenSpace(p0);
            ToScreenSpace(p1);
            ToScreenSpace(p2);
            DrawTriangle3(p0, p1, p2, pixel);
            return;
        }

        T1:
        {
            Vector3 b1 = v0;
            Vector3 b2 = v0;
            int outCode1; ClipLine(b1, v1, outCode1);
            int outCode2; ClipLine(b2, v2, outCode2);
            ToScreenSpace(b1);
            ToScreenSpace(b2);
            ToScreenSpace(v1);
            ToScreenSpace(v2);
            DrawTriangle3(b1, v1, b2, pixel);
            DrawTriangle3(b2, v1, v2, pixel);
            return;
        }

        T2:
        {
            int outCode1; ClipLine(v0, v1, outCode1);
            int outCode2; ClipLine(v0, v2, outCode2);
            ToScreenSpace(v0);
            ToScreenSpace(v1);
            ToScreenSpace(v2);
            DrawTriangle3(v0, v1, v2, pixel);
            return;
        }
    }
    void DrawTriangle3(Vector3 v0, Vector3 v1, Vector3 v2, Pixel pixel)
    {
        // p0 is top
        // p1 is middle
        // p2 is bottom
        if (v0.y > v1.y) swap(v0, v1);
        if (v1.y > v2.y) swap(v1, v2);
        if (v0.y > v1.y) swap(v0, v1);

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
        int err1 = dy1 / 2 - dx1abs;
        int err2 = dy2 / 2 - dx2abs;
        int err3 = dy3 / 2 - dx3abs;

        // check for 0 division?
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

        int cross = dx1 * dy2 - dy1 * dx2;

        int* xl; int* xr; float* zl; float* zr;
        if (cross < 0) { xl = &x1; xr = &x2; zl = &z1; zr = &z2; }
        else           { xl = &x2; xr = &x1; zl = &z2; zr = &z1; }

        for (int i = 0; i < dy2; i++)
        {
            while (err1 < 0) { err1 += dy1; x1 += dir1; }
            while (err2 < 0) { err2 += dy2; x2 += dir2; }
            DrawHorizontalLine2(y, *xl, *xr, *zl, *zr, pixel);
            y++;
            err1 -= dx1abs;
            err2 -= dx2abs;
            z1 += offset1;
            z2 += offset2;
        }
        for (int i = 0; i < dy3; i++)
        {
            while (err1 < 0) { err1 += dy1; x1 += dir1; }
            while (err3 < 0) { err3 += dy3; x2 += dir3; }
            DrawHorizontalLine2(y, *xl, *xr, *zl, *zr, pixel);
            y++;
            err1 -= dx1abs;
            err3 -= dx3abs;
            z1 += offset1;
            z2 += offset3;
        }
        DrawHorizontalLine2(y, *xl, *xr, *zl, *zr, pixel);
    }
    void DrawTriangle4(Vector3 v0, Vector3 v1, Vector3 v2, Pixel pixel)
    {
        auto st = StateTriangle(v0, v1, v2);

        for (size_t i = 0; i < st.dy; i++)
        {
            DrawHorizontalLine1(st.y, st.xl, st.xr, 0, 0, pixel);
            st.Update();
        }
        DrawHorizontalLine1(st.y, st.xl, st.xr, 0, 0, pixel);
    }

    void DrawPoligon(Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3, Pixel pixel)
    {
        DrawTriangle1(p0, p1, p2, pixel);
        DrawTriangle1(p2, p3, p0, pixel);
    }

    void ProjectLine(Vector3& v0, Vector3& v1, int& outCode)
    {
        v0.z -= nearZ;
        v1.z -= nearZ;
        ClipLineByZ4(v0, v1, outCode);
        if (outCode == 0) return;
        v0.z += nearZ;
        v1.z += nearZ;
        if (v0.z != 0) { v0.x /= v0.z; v0.y /= v0.z; };
        if (v1.z != 0) { v1.x /= v1.z; v1.y /= v1.z; };
    }

    void ProjectLine2(Vector3& v0, Vector3& v1)
    {
        if (v0.z != 0) { v0.x /= v0.z; v0.y /= v0.z; };
        if (v1.z != 0) { v1.x /= v1.z; v1.y /= v1.z; };
    }

    inline void DrawHorizontalLine1(int y, int xLeft, int xRight, float zLeft, float zRight, Pixel pixel)
    {
        // TODO remove
        if (xLeft > xRight)
        {
            swap(xLeft, xRight);
            swap(zLeft, zRight);
        }

        int count = xRight - xLeft;
        float diff = zRight - zLeft;
        float offset = diff / count;

        for (int i = 0; i < count + 1; i++)
        {
            auto x = xLeft + i;
            SetPixel2(x, y, zLeft, pixel);
            zLeft += offset;
        }
    }
    inline void DrawHorizontalLine2(int y, int xLeft, int xRight, float zLeft, float zRight, Pixel pixel)
    {
        int count = xRight - xLeft;
        float diff = zRight - zLeft;
        float offset = diff / count;

        for (int i = 0; i < count + 1; i++)
        {
            auto x = xLeft + i;
            SetPixel2(x, y, zLeft, pixel);
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
        // if (pixels[i] == BLACK)
        //     pixels[i] = pixel;
        // else
        //     pixels[i] = (pixels[i] + pixel) / 2;
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

            // if (pixels[i] == BLACK)
            //     pixels[i] = pixel;
            // else
            //     pixels[i] = (pixels[i] + pixel) / 2;

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

    void ApplyBlackWhiteColorDepth()
    {
        for (size_t i = 0; i < pixels.size(); i++)
        {
            float depthLength = 100;
            float factor = MathClamp(zbuffer[i], 0.0f, depthLength);
            factor /= depthLength;
            factor = 1 - factor;
            auto byte = (int)(factor * 255);

            uint32_t pixel = 0;
            pixel += byte; pixel = pixel << 8;
            pixel += byte; pixel = pixel << 8;
            pixel += byte; pixel = pixel << 8;
            pixel += byte;

            pixels[i] = pixel;

            // if (zbuffer[i] > 5)
            //     pixels[i] = BLACK;
        }
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
    void DrawTriangleSideCross1(Vector3 v0, Vector3 v1, Vector3 v2, Pixel pixel)
    {
        if (v0.y > v1.y) swap(v0, v1);
        if (v1.y > v2.y) swap(v1, v2);
        if (v0.y > v1.y) swap(v0, v1);

        auto d1 = v2 - v0;
        auto d2 = v1 - v0;

        float cross = d1.x * d2.y - d1.y * d2.x;

        if (cross > 0) swap(v1, v2);

        DrawTriangleSideCross2(v0, v1, v2, pixel);
    }
    void DrawTriangleSideCross2(Vector3 v0, Vector3 v1, Vector3 v2, Pixel pixel)
    {
        int x0 = (int)v0.x;
        int x1 = (int)v1.x;
        int x2 = (int)v2.x;

        int y0 = (int)v0.y;
        int y1 = (int)v1.y;
        int y2 = (int)v2.y;

        int minX = MathMin(MathMin(x0, x1), x2);
        int maxX = MathMax(MathMax(x0, x1), x2);
        int minY = MathMin(MathMin(y0, y1), y2);
        int maxY = MathMax(MathMax(y0, y1), y2);

        int xc = maxX - minX + 1;
        int yc = maxY - minY + 1;

        int dx0 = x1 - x0;
        int dx1 = x2 - x1;
        int dx2 = x0 - x2;

        int dy0 = y1 - y0;
        int dy1 = y2 - y1;
        int dy2 = y0 - y2;

        for (int x = minX; x < minX + xc; x++)
        for (int y = minY; y < minY + yc; y++)
        {
            int cross0 = dx0 * (y - y0) - dy0 * (x - x0); if (cross0 < 0) continue;
            int cross1 = dx1 * (y - y1) - dy1 * (x - x1); if (cross1 < 0) continue;
            int cross2 = dx2 * (y - y2) - dy2 * (x - x2); if (cross2 < 0) continue;

            SetPixel(x, y, pixel);
        }
    }
    void DrawLineTest1(Vector2Int p0, Vector2Int p1, Pixel pixel)
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
    void DrawLineTest2(Vector3 v0, Vector3 v1, Pixel pixel)
    {
        if (v0.y > v1.y) swap(v0, v1);

        Vector2Int p0 = { (int)v0.x, (int)v0.y };
        Vector2Int p1 = { (int)v1.x, (int)v1.y };

        int dy = p1.y - p0.y;

        int dx, sx;
        if (p0.x < p1.x) { dx = p1.x - p0.x; sx =  1; }
        else             { dx = p0.x - p1.x; sx = -1; }

        int max, min;
        int err;

        if (dx > dy) { err =  dx / 2; max = dx; min = dy; }
        else         { err = -dy / 2; max = dy; min = dx; }

        for (int i = 0; i < dy + 1; i++)
        {
            while (err > max)
            {
                err -= dy;
                p0.x += sx;
                SetPixel(p0.x, p0.y, pixel);
            }
            SetPixel(p0.x, p0.y, pixel);
            p0.y++;
            err += dx;
        }
    }

private:
    int width = 0;
    int height = 0;
};
