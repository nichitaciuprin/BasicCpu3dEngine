#pragma once

// functions made for specific case
// maybe remove hardcoded values

bool ClipLineBack(Vector3& v0, Vector3& v1)
{
    float offset = 0.1f;

    int flags = 0;

    if (v0.z < offset) flags += 1;
    if (v1.z < offset) flags += 2;

    switch (flags)
    {
        case /* 00 */ 0: return false;
        case /* 01 */ 1:
        {
            auto diff = v0 - v1;
            v0 = v0 + diff * (offset - v0.z) / diff.z;
            v0.z = offset;
            return false;
        }
        case /* 10 */ 2:
        {
            auto diff = v1 - v0;
            v1 = v1 + diff * (offset - v1.z) / diff.z;
            v1.z = offset;
            return false;
        }
        default /* 11 */ : return true;
    }
}
bool ClipLineLeft(Vector3& v0, Vector3& v1)
{
    float offset = -1;

    int flags = 0;

    if (v0.x < offset) flags += 1;
    if (v1.x < offset) flags += 2;

    switch (flags)
    {
        case /* 00 */ 0: return false;
        case /* 01 */ 1:
        {
            auto diff = v0 - v1;
            v0 = v0 + diff * (offset - v0.x) / diff.x;
            v0.x = offset;
            return false;
        }
        case /* 10 */ 2:
        {
            auto diff = v1 - v0;
            v1 = v1 + diff * (offset - v1.x) / diff.x;
            v1.x = offset;
            return false;
        }
        default /* 11 */ : return true;
    }
}
bool ClipLineRight(Vector3& v0, Vector3& v1)
{
    float offset = 1;

    int flags = 0;

    if (v0.x > offset) flags += 1;
    if (v1.x > offset) flags += 2;

    switch (flags)
    {
        case /* 00 */ 0: return false;
        case /* 01 */ 1:
        {
            auto diff = v0 - v1;
            v0 = v0 + diff * (offset - v0.x) / diff.x;
            v0.x = offset;
            return false;
        }
        case /* 10 */ 2:
        {
            auto diff = v1 - v0;
            v1 = v1 + diff * (offset - v1.x) / diff.x;
            v1.x = offset;
            return false;
        }
        default /* 11 */ : return true;
    }
}
bool ClipLineTop(Vector3& v0, Vector3& v1)
{
    float offset = 1;

    int flags = 0;

    if (v0.y > offset) flags += 1;
    if (v1.y > offset) flags += 2;

    switch (flags)
    {
        case /* 00 */ 0: return false;
        case /* 01 */ 1:
        {
            auto diff = v0 - v1;
            v0 = v0 + diff * (offset - v0.y) / diff.y;
            v0.y = offset;
            return false;
        }
        case /* 10 */ 2:
        {
            auto diff = v1 - v0;
            v1 = v1 + diff * (offset - v1.y) / diff.y;
            v1.y = offset;
            return false;
        }
        default /* 11 */ : return true;
    }
}
bool ClipLineBottom(Vector3& v0, Vector3& v1)
{
    float offset = -1;

    int flags = 0;

    if (v0.y < offset) flags += 1;
    if (v1.y < offset) flags += 2;

    switch (flags)
    {
        case /* 00 */ 0: return false;
        case /* 01 */ 1:
        {
            auto diff = v0 - v1;
            v0 = v0 + diff * (offset - v0.y) / diff.y;
            v0.y = offset;
            return false;
        }
        case /* 10 */ 2:
        {
            auto diff = v1 - v0;
            v1 = v1 + diff * (offset - v1.y) / diff.y;
            v1.y = offset;
            return false;
        }
        default /* 11 */ : return true;
    }
}

void ClipPoligonBack(vector<Vector3>& input, vector<Vector3>& output)
{
    float offset = 0.1f;
    int flags = 0;

    Vector3 p0 = input[input.size() - 1];
    if (p0.z < offset) flags += 2;

    for (size_t i = 0; i < input.size(); i++)
    {
        flags = flags >> 1;

        Vector3 p1 = input[i];
        if (p1.z < offset) flags += 2;

        switch (flags)
        {
            /* 00 */ case 0:
            {
                output.push_back(p0);
                break;
            };
            /* 10 */ case 2:
            {
                output.push_back(p0);
                auto diff = p1 - p0;
                auto newPoint = p0 + diff * (offset - p0.z) / diff.z;
                newPoint.z = offset;
                output.push_back(newPoint);
                break;
            };
            /* 01 */ case 1:
            {
                auto diff = p1 - p0;
                auto newPoint = p0 + diff * (offset - p0.z) / diff.z;
                newPoint.z = offset;
                output.push_back(newPoint);
                break;
            };
            default: break;
        }

        p0 = p1;
    }
}
void ClipPoligonLeft(vector<Vector3>& input, vector<Vector3>& output)
{
    float offset = -1;
    int flags = 0;

    Vector3 p0 = input[input.size() - 1];
    if (p0.x < offset) flags += 2;

    for (size_t i = 0; i < input.size(); i++)
    {
        flags = flags >> 1;

        Vector3 p1 = input[i];
        if (p1.x < offset) flags += 2;

        switch (flags)
        {
            /* 00 */ case 0:
            {
                output.push_back(p0);
                break;
            };
            /* 10 */ case 2:
            {
                output.push_back(p0);
                auto diff = p1 - p0;
                auto newPoint = p0 + diff * (offset - p0.x) / diff.x;
                newPoint.x = offset;
                output.push_back(newPoint);
                break;
            };
            /* 01 */ case 1:
            {
                auto diff = p1 - p0;
                auto newPoint = p0 + diff * (offset - p0.x) / diff.x;
                newPoint.x = offset;
                output.push_back(newPoint);
                break;
            };
            default: break;
        }

        p0 = p1;
    }
}
void ClipPoligonRight(vector<Vector3>& input, vector<Vector3>& output)
{
    float offset = 1;
    int flags = 0;

    Vector3 p0 = input[input.size() - 1];
    if (p0.x > offset) flags += 2;

    for (size_t i = 0; i < input.size(); i++)
    {
        flags = flags >> 1;

        Vector3 p1 = input[i];
        if (p1.x > offset) flags += 2;

        switch (flags)
        {
            /* 00 */ case 0:
            {
                output.push_back(p0);
                break;
            };
            /* 10 */ case 2:
            {
                output.push_back(p0);
                auto diff = p1 - p0;
                auto newPoint = p0 + diff * (offset - p0.x) / diff.x;
                newPoint.x = offset;
                output.push_back(newPoint);
                break;
            };
            /* 01 */ case 1:
            {
                auto diff = p1 - p0;
                auto newPoint = p0 + diff * (offset - p0.x) / diff.x;
                newPoint.x = offset;
                output.push_back(newPoint);
                break;
            };
            default: break;
        }

        p0 = p1;
    }
}
void ClipPoligonTop(vector<Vector3>& input, vector<Vector3>& output)
{
    float offset = 1;
    int flags = 0;

    Vector3 p0 = input[input.size() - 1];
    if (p0.y > offset) flags += 2;

    for (size_t i = 0; i < input.size(); i++)
    {
        flags = flags >> 1;

        Vector3 p1 = input[i];
        if (p1.y > offset) flags += 2;

        switch (flags)
        {
            /* 00 */ case 0:
            {
                output.push_back(p0);
                break;
            };
            /* 10 */ case 2:
            {
                output.push_back(p0);
                auto diff = p1 - p0;
                auto newPoint = p0 + diff * (offset - p0.y) / diff.y;
                newPoint.y = offset;
                output.push_back(newPoint);
                break;
            };
            /* 01 */ case 1:
            {
                auto diff = p1 - p0;
                auto newPoint = p0 + diff * (offset - p0.y) / diff.y;
                newPoint.y = offset;
                output.push_back(newPoint);
                break;
            };
            default: break;
        }

        p0 = p1;
    }
}
void ClipPoligonBottom(vector<Vector3>& input, vector<Vector3>& output)
{
    float offset = -1;
    int flags = 0;

    Vector3 p0 = input[input.size() - 1];
    if (p0.y < offset) flags += 2;

    for (size_t i = 0; i < input.size(); i++)
    {
        flags = flags >> 1;

        Vector3 p1 = input[i];
        if (p1.y < offset) flags += 2;

        switch (flags)
        {
            /* 00 */ case 0:
            {
                output.push_back(p0);
                break;
            };
            /* 10 */ case 2:
            {
                output.push_back(p0);
                auto diff = p1 - p0;
                auto newPoint = p0 + diff * (offset - p0.y) / diff.y;
                newPoint.y = offset;
                output.push_back(newPoint);
                break;
            };
            /* 01 */ case 1:
            {
                auto diff = p1 - p0;
                auto newPoint = p0 + diff * (offset - p0.y) / diff.y;
                newPoint.y = offset;
                output.push_back(newPoint);
                break;
            };
            default: break;
        }

        p0 = p1;
    }
}

// maybe better line clipping

int GetPointState(float x, float y)
{
    const int INSIDE = 0; // 0000
    const int LEFT   = 1; // 0001
    const int RIGHT  = 2; // 0010
    const int BOTTOM = 4; // 0100
    const int TOP    = 8; // 1000

    const int xmin = -1;
    const int xmax =  1;
    const int ymin = -1;
    const int ymax =  1;

	int code = INSIDE;

	if      (x < xmin) code |= LEFT;
	else if (x > xmax) code |= RIGHT;
	if      (y < ymin) code |= BOTTOM;
	else if (y > ymax) code |= TOP;

	return code;
}
void ClipLine(Vector3& p0, Vector3& p1, int& outCode)
{
    const int LEFT   = 1; // 0001
    const int RIGHT  = 2; // 0010
    const int BOTTOM = 4; // 0100
    const int TOP    = 8; // 1000

    const int xmin = -1;
    const int xmax =  1;
    const int ymin = -1;
    const int ymax =  1;

	int code0 = GetPointState(p0.x, p0.y);
	int code1 = GetPointState(p1.x, p1.y);

	while (true)
    {
		if (!(code0 | code1)) { outCode = 0; return; } // points inside
        if (  code0 & code1 ) { outCode = 1; return; } // points in same outside zone

        int code =
            code0 > code1 ?
            code0 : code1;

        float x = 0;
        float y = 0;

        if      (code & LEFT)   { y = p0.y + (p1.y - p0.y) * (xmin - p0.x) / (p1.x - p0.x); x = xmin; } // point is to the left of clip window
        else if (code & RIGHT)  { y = p0.y + (p1.y - p0.y) * (xmax - p0.x) / (p1.x - p0.x); x = xmax; } // point is to the right of clip window
        if      (code & BOTTOM) { x = p0.x + (p1.x - p0.x) * (ymin - p0.y) / (p1.y - p0.y); y = ymin; } // point is below the clip window
        else if (code & TOP)    { x = p0.x + (p1.x - p0.x) * (ymax - p0.y) / (p1.y - p0.y); y = ymax; } // point is above the clip window

        if (code == code0) { p0.x = x; p0.y = y; code0 = GetPointState(p0.x, p0.y); }
        else               { p1.x = x; p1.y = y; code1 = GetPointState(p1.x, p1.y); }
	}
}