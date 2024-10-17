// http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html
// https://developer.nvidia.com/gpugems/gpugems2/part-v-image-oriented-computing/chapter-42-conservative-rasterization

#pragma once

// TOOD maybe find better place for colors

typedef uint32_t Pixel;

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

uint32_t PixelToBwPixel(uint32_t pixel)
{
    int r = (uint8_t)((pixel >> 8 * 2));
    int g = (uint8_t)((pixel >> 8 * 1));
    int b = (uint8_t)((pixel >> 8 * 0));

    uint8_t value = ((r + g + b) / 3);
    float fraction = (float)value / 255;

    return 0x00FFFFFF * fraction;
}
uint8_t PixelToLightValue(uint32_t pixel)
{
    int r = (uint8_t)((pixel >> 8 * 2));
    int g = (uint8_t)((pixel >> 8 * 1));
    int b = (uint8_t)((pixel >> 8 * 0));

    uint8_t value = ((r + g + b) / 3);

    return value;
}
uint32_t LightValueToPixel(uint8_t pixel)
{
    float fraction = (float)pixel / 255;
    return 0x00FFFFFF * fraction;
}

class Bitmap
{
public:
    vector<uint32_t> pixels;
    vector<float> zbuffer;

    Bitmap(int width, int height)
    {
        if (width < 1) throw "width < 1";
        if (height < 1) throw "height < 1";

        this->width = width;
        this->height = height;

        widthMin1 = width - 1;
        heightMin1 = height - 1;

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

    void DrawCube(Vector3 position, Vector3 rotation, Camera camera, Pixel pixel)
    {
        Vector3 scale = { 1, 1, 1 };
        auto world = MatrixWorld(position, rotation, scale);
        auto view = MatrixView(&camera);
        DrawCube(world * view, pixel);
    }

    void DrawCubeWireframe(Matrix modelView, Pixel pixel)
    {
        int indices[12][2] =
        {
            0, 1,
            1, 5,
            5, 4,
            4, 0,
            2, 3,
            3, 7,
            7, 6,
            6, 2,
            0, 2,
            1, 3,
            5, 7,
            4, 6
        };

        for (size_t i = 0; i < 12; i++)
        {
            auto i0 = indices[i][0];
            auto i1 = indices[i][1];
            auto v0 = Model::Cube::vertices[i0] * modelView;
            auto v1 = Model::Cube::vertices[i1] * modelView;
            DrawLine1(v0, v1, pixel);
        }
    }
    void DrawCubeColored(Matrix modelView)
    {
        int indexData[6][4] =
        {
            2, 6, 4, 0,
            6, 7, 5, 4,
            7, 3, 1, 5,
            3, 2, 0, 1,
            1, 0, 4, 5,
            3, 7, 6, 2,
        };

        #define DRAW(INDEX, COLOR)                \
        {                                         \
            auto i0 = indexData[INDEX][0];        \
            auto i1 = indexData[INDEX][1];        \
            auto i2 = indexData[INDEX][2];        \
            auto i3 = indexData[INDEX][3];        \
            auto p0 = Model::Cube::vertices[i0];  \
            auto p1 = Model::Cube::vertices[i1];  \
            auto p2 = Model::Cube::vertices[i2];  \
            auto p3 = Model::Cube::vertices[i3];  \
            p0 *= modelView;                      \
            p1 *= modelView;                      \
            p2 *= modelView;                      \
            p3 *= modelView;                      \
            DrawPoligon1(p0, p1, p2, p3, COLOR);  \
        }                                         \

        DRAW(0, CYAN)
        DRAW(1, GREEN)
        DRAW(2, BLUE)
        DRAW(3, YELLOW)
        DRAW(4, MAGENTA)
        DRAW(5, RED)

        #undef DRAW
    }
    void DrawCube(Matrix modelView, Pixel color)
    {
        int indexData[6][4] =
        {
            2, 6, 4, 0,
            6, 7, 5, 4,
            7, 3, 1, 5,
            3, 2, 0, 1,
            1, 0, 4, 5,
            3, 7, 6, 2,
        };

        #define DRAW(INDEX, COLOR)                \
        {                                         \
            auto i0 = indexData[INDEX][0];        \
            auto i1 = indexData[INDEX][1];        \
            auto i2 = indexData[INDEX][2];        \
            auto i3 = indexData[INDEX][3];        \
            auto p0 = Model::Cube::vertices[i0];  \
            auto p1 = Model::Cube::vertices[i1];  \
            auto p2 = Model::Cube::vertices[i2];  \
            auto p3 = Model::Cube::vertices[i3];  \
            p0 *= modelView;                      \
            p1 *= modelView;                      \
            p2 *= modelView;                      \
            p3 *= modelView;                      \
            DrawPoligon1(p0, p1, p2, p3, COLOR);  \
        }                                         \

        DRAW(0, color)
        DRAW(1, color)
        DRAW(2, color)
        DRAW(3, color)
        DRAW(4, color)
        DRAW(5, color)

        #undef DRAW
    }
    void DrawPoligon1(Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3, Pixel pixel)
    {
        auto v0 = vector<Vector3>();
        auto v1 = vector<Vector3>();

        v0.reserve(6);
        v1.reserve(6);

        v0.push_back(p0);
        v0.push_back(p1);
        v0.push_back(p2);
        v0.push_back(p3);

        ClipPoligonBack      (v0, v1); if (v1.size() < 3) return; v0.clear();

        for (auto& x : v1)
        {
            if (x.z == 0) continue;
            x.x /= x.z;
            x.y /= x.z;
        }

        if (!Vector3TriangleIsClockwise(v1[0], v1[1], v1[2])) return;

        ClipPoligonLeft      (v1, v0); if (v0.size() < 3) return; v1.clear();
        ClipPoligonRight     (v0, v1); if (v1.size() < 3) return; v0.clear();
        ClipPoligonTop       (v1, v0); if (v0.size() < 3) return; v1.clear();
        ClipPoligonBottom    (v0, v1); if (v1.size() < 3) return;

        for (auto& x : v1)
            ToScreenSpace(x);

        for (size_t i = 1; i < v1.size() - 1; i++)
            DrawTriangle(v1[0], v1[i], v1[i + 1], pixel);
    }
    void DrawTriangle1(Vector3 p0, Vector3 p1, Vector3 p2, Pixel pixel)
    {
        auto v0 = vector<Vector3>();
        auto v1 = vector<Vector3>();

        v0.reserve(6);
        v1.reserve(6);

        v0.push_back(p0);
        v0.push_back(p1);
        v0.push_back(p2);

        ClipPoligonBack   (v0, v1); if (v1.size() < 3) return; v0.clear();

        for (auto& x : v1)
        {
            if (x.z == 0) continue;
            x.x /= x.z;
            x.y /= x.z;
        }

        if (!Vector3TriangleIsClockwise(v1[0], v1[1], v1[2])) return;

        ClipPoligonLeft   (v1, v0); if (v0.size() < 3) return; v1.clear();
        ClipPoligonRight  (v0, v1); if (v1.size() < 3) return; v0.clear();
        ClipPoligonTop    (v1, v0); if (v0.size() < 3) return; v1.clear();
        ClipPoligonBottom (v0, v1); if (v1.size() < 3) return;

        for (auto& x : v1)
            ToScreenSpace(x);

        for (size_t i = 1; i < v1.size() - 1; i++)
            DrawTriangle(v1[0], v1[i], v1[i + 1], pixel);
    }
    void DrawTriangle2(Vector3 p0, Vector3 p1, Vector3 p2, Pixel pixel)
    {
        auto v0 = vector<Vector3>();
        auto v1 = vector<Vector3>();

        v0.reserve(6);
        v1.reserve(6);

        v0.push_back(p0);
        v0.push_back(p1);
        v0.push_back(p2);

        ClipPoligonBack(v0, v1); if (v1.size() < 3) return; v0.clear();

        if (!Vector3TriangleIsClockwise(v1[0], v1[1], v1[2])) return;

        for (auto& x : v1)
            ToScreenSpace(x);

        for (size_t i = 1; i < v1.size() - 1; i++)
            DrawTriangle(v1[0], v1[i], v1[i + 1], pixel);
    }
    void DrawLine1(Vector3 v0, Vector3 v1, Pixel pixel)
    {
        if (ClipLineBack(v0, v1)) return;

        if (v0.z != 0) { v0.x /= v0.z; v0.y /= v0.z; };
        if (v1.z != 0) { v1.x /= v1.z; v1.y /= v1.z; };

        if (ClipLineLeft(v0, v1)) return;
        if (ClipLineRight(v0, v1)) return;
        if (ClipLineTop(v0, v1)) return;
        if (ClipLineBottom(v0, v1)) return;

        ToScreenSpace(v0);
        ToScreenSpace(v1);

        DrawLine(v0, v1, pixel);
    }
    void DrawLine2(Vector3 v0, Vector3 v1, Pixel pixel)
    {
        if (ClipLineLeft(v0, v1)) return;
        if (ClipLineRight(v0, v1)) return;
        if (ClipLineTop(v0, v1)) return;
        if (ClipLineBottom(v0, v1)) return;

        ToScreenSpace(v0);
        ToScreenSpace(v1);

        DrawLine(v0, v1, pixel);
    }
    void ToScreenSpace(Vector3& point)
    {
        point.y = -point.y;
        point.x += 1.0f;
        point.y += 1.0f;
        point.x /= 2;
        point.y /= 2;
        point.x = widthMin1 * point.x;
        point.y = heightMin1 * point.y;
    }

    void Fill(Pixel pixel)
    {
        // TODO move to BeginDraw()
        fill(zbuffer.begin(), zbuffer.end(), 100000000.0f);
        fill(pixels.begin(), pixels.end(), pixel);
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
        }
    }
    void DrawTriangle(Vector3 v0, Vector3 v1, Vector3 v2, Pixel pixel)
    {
        // TODO not accurate, improve

        // p0 is top
        // p1 is middle
        // p2 is bottom
        if (v0.y > v1.y) swap(v0, v1);
        if (v1.y > v2.y) swap(v1, v2);
        if (v0.y > v1.y) swap(v0, v1);

        // TODO maybe remove Vector2Int
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

        // TODO check for 0 division?
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
            DrawLineHorizontal(y, *xl, *xr, *zl, *zr, pixel);
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
            DrawLineHorizontal(y, *xl, *xr, *zl, *zr, pixel);
            y++;
            err1 -= dx1abs;
            err3 -= dx3abs;
            z1 += offset1;
            z2 += offset3;
        }
        DrawLineHorizontal(y, *xl, *xr, *zl, *zr, pixel);
    }
    void DrawLine(Vector3 v0, Vector3 v1, Pixel pixel)
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
            SetPixelZ(x0, y0, z, pixel);
            if (err < 0) { err += max; (*axis1) += val1; }
                         { err -= min; (*axis2) += val2; }
            z += offset;
        }
        SetPixelZ(x0, y0, z, pixel);
    }
    void DrawLineHorizontal(int y, int xLeft, int xRight, float zLeft, float zRight, Pixel pixel)
    {
        int count = xRight - xLeft;
        float diff = zRight - zLeft;
        float offset = diff / count;

        for (int i = 0; i < count + 1; i++)
        {
            auto x = xLeft + i;
            SetPixelZ(x, y, zLeft, pixel);
            zLeft += offset;
        }
    }
    void DrawBorder(Pixel pixel)
    {
        int x = widthMin1;
        int y = heightMin1;
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
    void SetPixel(int x, int y, Pixel pixel)
    {
        auto i = x + y * width;
        pixels[i] = pixel;
    }
    void SetPixelZ(int x, int y, float z, Pixel pixel)
    {
        auto i = x + y * width;
        if (zbuffer[i] >= z)
        {
            zbuffer[i] = z;
            pixels[i] = pixel;
        }
    }

private:
    int width = 0;
    int height = 0;
    int widthMin1 = 0;
    int heightMin1 = 0;
};
