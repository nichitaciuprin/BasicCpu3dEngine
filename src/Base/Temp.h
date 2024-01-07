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