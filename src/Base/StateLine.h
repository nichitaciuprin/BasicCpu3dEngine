#pragma once

struct StateLine
{
    float offset;
    float z;

    int x0; int y0;
    int x1; int y1;

    int dx; int dy;
    int sx; int sy;

    int err;

    void (*step)(StateLine* lt);

    StateLine() {}

    StateLine(Vector3 p0, Vector3 p1)
    {
        x0 = (int)p0.x;
        x1 = (int)p1.x;
        y0 = (int)p0.y;
        y1 = (int)p1.y;
        float z0 = p0.z;
        float z1 = p1.z;

        if (x0 < x1) { dx = x1 - x0; sx =  1; }
        else         { dx = x0 - x1; sx = -1; }

        if (y0 < y1) { dy = y1 - y0; sy =  1; }
        else         { dy = y0 - y1; sy = -1; }

        if (dx > dy)
        {
            err = dx / 2 - dy;
            step = &DrawStepDX;
            offset = (z1 - z0) / dx;
        }
        else
        {
            err = dy / 2 - dx;
            step = &DrawStepDY;
            offset = (z1 - z0) / dy;
        }

        z = z0;
    }
    void Update()
    {
        step(this);
        z += offset;
    }
    static void DrawStepDX(StateLine* lt)
    {
        if (lt->err < 0)
        { lt->err += lt->dx; lt->y0 += lt->sy; }
        { lt->err -= lt->dy; lt->x0 += lt->sx; }
    }
    static void DrawStepDY(StateLine* lt)
    {
        if (lt->err < 0)
        { lt->err += lt->dy; lt->x0 += lt->sx; }
        { lt->err -= lt->dx; lt->y0 += lt->sy; }
    }
};

