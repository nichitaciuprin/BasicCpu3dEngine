#pragma once

struct StateTriangle
{
    StateLine lt0;
    StateLine lt1;
    StateLine lt2;

    int xl;
    int xr;
    int y;

    StateTriangle(Vector3 v0, Vector3 v1, Vector3 v2)
    {
        if (v0.y > v1.y) swap(v0, v1);
        if (v1.y > v2.y) swap(v1, v2);
        if (v0.y > v1.y) swap(v0, v1);

        int x0 = (int)v0.x;
        int x1 = (int)v1.x;
        int x2 = (int)v2.x;
        int y0 = (int)v0.y;
        int y1 = (int)v1.y;
        int y2 = (int)v2.y;
        float z0 = v0.z;
        float z1 = v1.z;
        float z2 = v2.z;

        int cross = lt0.dx * lt1.dy - lt0.dy * lt1.dx;

        // int x1 = p0.x;
        // float z1 = v0.z;

        // int x2;
        // float z2;
        // if (dy2 > 0) { x2 = p0.x; z2 = v0.z; }
        // else         { x2 = p1.x; z2 = v1.z; }

        lt0 = StateLine(x0, y0, x2, y2, z0, z2);
        lt1 = StateLine(x0, y0, x1, y1, z0, z1);
        lt2 = StateLine(x1, y1, x2, y2, z1, z2);

        xl = lt0.x0;
        xr = lt0.x0;
        y = lt0.y0;
    }
    void Update1()
    {
        do { lt0.Update(); } while (y == lt0.y0); xl = lt0.x0;
        do { lt1.Update(); } while (y == lt1.y0); xr = lt1.x0;
        y++;
    }
    void Update2()
    {
        do { lt0.Update(); } while (y == lt0.y0); xl = lt0.x0;
        do { lt2.Update(); } while (y == lt2.y0); xr = lt2.x0;
        y++;
    }
};

