#pragma once

// void ClipPoligon(Vector3* points, int* pointsCount)
// {

// }
void ClipTriangleRight(Vector3& p0, Vector3& p1, Vector3& p2, Vector3& p3, int& outCode)
{
    int offset = 1;

    int state = 0;

    if (p2.x > offset) state += 1;
    if (p1.x > offset) state += 2;
    if (p0.x > offset) state += 4;

    switch (state)
    {
        /* 000 */ case 0: { outCode = 0; return; };
        /* 111 */ case 7: { outCode = 3; return; };
        default: break;
    }

    Vector3 v0, v1, v2;

    switch (state)
    {
        /* 001 */ case 1: { v0 = p2; v1 = p0; v2 = p1; goto POINT_1; };
        /* 010 */ case 2: { v0 = p1; v1 = p2; v2 = p0; goto POINT_1; };
        /* 100 */ case 4: { v0 = p0; v1 = p1; v2 = p2; goto POINT_1; };

        /* 011 */ case 3: { v0 = p0; v1 = p1; v2 = p2; goto POINT_2; };
        /* 101 */ case 5: { v0 = p1; v1 = p2; v2 = p0; goto POINT_2; };
        /* 110 */ case 6: { v0 = p2; v1 = p0; v2 = p1; goto POINT_2; };

        default: __assume(0);
    }

    POINT_1:
    {
        outCode = 1;
        p0 = { 1, v0.y + (v2.y - v0.y) * (offset - v0.x) / (v2.x - v0.x) };
        p1 = { 1, v0.y + (v1.y - v0.y) * (offset - v0.x) / (v1.x - v0.x) };
        p2 = v1;
        p3 = v2;
        return;
    }

    POINT_2:
    {
        outCode = 2;
        p0 = v0;
        p1 = { 1, v1.y + (v0.y - v1.y) * (offset - v1.x) / (v1.x - v0.x) };
        p2 = { 1, v2.y + (v0.y - v2.y) * (offset - v2.x) / (v2.x - v0.x) };
        return;
    }
}
void ClipTriangleLeft(Vector3& p0, Vector3& p1, Vector3& p2, Vector3& p3, int& outCode)
{
    int offset = -1;

    int state = 0;

    if (p2.x < offset) state += 1;
    if (p1.x < offset) state += 2;
    if (p0.x < offset) state += 4;

    switch (state)
    {
        /* 000 */ case 0: { outCode = 0; return; };
        /* 111 */ case 7: { outCode = 3; return; };
        default: break;
    }

    Vector3 v0, v1, v2;

    switch (state)
    {
        /* 001 */ case 1: { v0 = p2; v1 = p0; v2 = p1; goto POINT_1; };
        /* 010 */ case 2: { v0 = p1; v1 = p2; v2 = p0; goto POINT_1; };
        /* 100 */ case 4: { v0 = p0; v1 = p1; v2 = p2; goto POINT_1; };

        /* 011 */ case 3: { v0 = p0; v1 = p1; v2 = p2; goto POINT_2; };
        /* 101 */ case 5: { v0 = p1; v1 = p2; v2 = p0; goto POINT_2; };
        /* 110 */ case 6: { v0 = p2; v1 = p0; v2 = p1; goto POINT_2; };

        default: __assume(0);
    }

    POINT_1:
    {
        outCode = 1;
        p0 = { 1, v0.y + (v2.y - v0.y) * (offset - v0.x) / (v2.x - v0.x) };
        p1 = { 1, v0.y + (v1.y - v0.y) * (offset - v0.x) / (v1.x - v0.x) };
        p2 = v1;
        p3 = v2;
        return;
    }

    POINT_2:
    {
        outCode = 2;
        p0 = v0;
        p1 = { 1, v1.y + (v0.y - v1.y) * (offset - v1.x) / (v1.x - v0.x) };
        p2 = { 1, v2.y + (v0.y - v2.y) * (offset - v2.x) / (v2.x - v0.x) };
        return;
    }
}