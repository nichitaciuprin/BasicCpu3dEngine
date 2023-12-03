#pragma once

struct StateTriangle
{
    StateLine lt0;
    StateLine lt1;
    StateLine lt2;

    int y;
    int xl;
    int xr;
    float zl;
    float zr;

    int dy;

    void (*step)(StateTriangle* lt);

    StateTriangle(Vector3 v0, Vector3 v1, Vector3 v2)
    {
        if (v0.y > v1.y) swap(v0, v1);
        if (v1.y > v2.y) swap(v1, v2);
        if (v0.y > v1.y) swap(v0, v1);

        int x0 = (int)v0.x;
        int x1 = (int)v1.x;
        // int x2 = (int)v2.x;
        int y0 = (int)v0.y;
        int y1 = (int)v1.y;
        int y2 = (int)v2.y;
        float z0 = v0.z;
        float z1 = v1.z;
        // float z2 = v2.z;

        dy = y2 - y0;

        y = y0;

        if (y0 == y1)
        {
            step = &Update2;
            lt0 = StateLine(v0, v2);
            lt2 = StateLine(v1, v2);
            xl = x0;
            xr = x1;
            zl = z0;
            zr = z1;
        }
        else
        {
            step = &Update1;
            lt0 = StateLine(v0, v2);
            lt1 = StateLine(v0, v1);
            lt2 = StateLine(v1, v2);
            xl = x0;
            xr = x0;
            zl = z0;
            zr = z0;
        }

        // int cross = lt0.dx * lt1.dy - lt0.dy * lt1.dx;
        // if (cross < 0)
    }
    void Update()
    {
        step(this);
    }
    static void Update1(StateTriangle* obj)
    {
        do { obj->lt0.Update(); } while (obj->y == obj->lt0.y0); obj->xl = obj->lt0.x0; obj->zl = obj->lt0.z;
        do { obj->lt1.Update(); } while (obj->y == obj->lt1.y0); obj->xr = obj->lt1.x0; obj->zr = obj->lt1.z;
        obj->y++;
        if (obj->y == obj->lt1.y1)
            obj->step = &Update2;
    }
    static void Update2(StateTriangle* obj)
    {
        do { obj->lt0.Update(); } while (obj->y == obj->lt0.y0); obj->xl = obj->lt0.x0; obj->zl = obj->lt0.z;
        do { obj->lt2.Update(); } while (obj->y == obj->lt2.y0); obj->xr = obj->lt2.x0; obj->zr = obj->lt2.z;
        obj->y++;
    }
};

