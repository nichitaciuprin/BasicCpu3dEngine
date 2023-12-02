#pragma once

struct LineState
{
    float offset;
    float z;

    int x0; int y0;
    int x1; int y1;

    int dx; int dy;
    int sx; int sy;

    int err;

    void (*step)(LineState* lt);

    LineState(int _x0, int _y0, int _x1, int _y1, float _z0, float _z1)
    {
        x0 = _x0;
        y0 = _y0;
        x1 = _x1;
        y1 = _y1;

        if (x0 < x1) { dx = x1 - x0; sx =  1; }
        else         { dx = x0 - x1; sx = -1; }

        if (y0 < y1) { dy = y1 - y0; sy =  1; }
        else         { dy = y0 - y1; sy = -1; }

        if (dx > dy)
        {
            err = dx / 2 - dy;
            step = &DrawStepDX;
            offset = (_z1 - _z0) / dx;
        }
        else
        {
            err = dy / 2 - dx;
            step = &DrawStepDY;
            offset = (_z1 - _z0) / dy;
        }

        z = _z0;
    }
    void Update()
    {
        // cout << x0 << endl;
        step(this);
        z += offset;
    }
    static void DrawStepDX(LineState* lt)
    {
        if (lt->err < 0)
        { lt->err += lt->dx; lt->y0 += lt->sy; }
        { lt->err -= lt->dy; lt->x0 += lt->sx; }
    }
    static void DrawStepDY(LineState* lt)
    {
        if (lt->err < 0)
        { lt->err += lt->dy; lt->x0 += lt->sx; }
        { lt->err -= lt->dx; lt->y0 += lt->sy; }
    }
};

