#pragma once

// functions made for specific case
// maybe remove hardcoded values

bool ClipLineBack(Vector3& v0, Vector3& v1)
{
    float offset = 0;

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
    int state = 0;

    Vector3 p0 = input[input.size() - 1];
    if (p0.z < offset) state += 2;

    for (int i = 0; i < input.size(); i++)
    {
        state = state >> 1;

        Vector3 p1 = input[i];
        if (p1.z < offset) state += 2;

        switch (state)
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
                auto newPoint = (p0 + diff * (offset - p0.z) / diff.z);
                newPoint.z = offset;
                output.push_back(newPoint);
                break;
            };
            /* 01 */ case 1:
            {
                auto diff = p1 - p0;
                auto newPoint = (p0 + diff * (offset - p0.z) / diff.z);
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
    int state = 0;

    Vector3 p0 = input[input.size() - 1];
    if (p0.x < offset) state += 2;

    for (size_t i = 0; i < input.size(); i++)
    {
        state = state >> 1;

        Vector3 p1 = input[i];
        if (p1.x < offset) state += 2;

        switch (state)
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
                auto newPoint = (p0 + diff * (offset - p0.x) / diff.x);
                newPoint.x = offset;
                output.push_back(newPoint);
                break;
            };
            /* 01 */ case 1:
            {
                auto diff = p1 - p0;
                auto newPoint = (p0 + diff * (offset - p0.x) / diff.x);
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
    int state = 0;

    Vector3 p0 = input[input.size() - 1];
    if (p0.x > offset) state += 2;

    for (size_t i = 0; i < input.size(); i++)
    {
        state = state >> 1;

        Vector3 p1 = input[i];
        if (p1.x > offset) state += 2;

        switch (state)
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
                auto newPoint = (p0 + diff * (offset - p0.x) / diff.x);
                newPoint.x = offset;
                output.push_back(newPoint);
                break;
            };
            /* 01 */ case 1:
            {
                auto diff = p1 - p0;
                auto newPoint = (p0 + diff * (offset - p0.x) / diff.x);
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
    int state = 0;

    Vector3 p0 = input[input.size() - 1];
    if (p0.y > offset) state += 2;

    for (size_t i = 0; i < input.size(); i++)
    {
        state = state >> 1;

        Vector3 p1 = input[i];
        if (p1.y > offset) state += 2;

        switch (state)
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
                auto newPoint = (p0 + diff * (offset - p0.y) / diff.y);
                newPoint.y = offset;
                output.push_back(newPoint);
                break;
            };
            /* 01 */ case 1:
            {
                auto diff = p1 - p0;
                auto newPoint = (p0 + diff * (offset - p0.y) / diff.y);
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
    int state = 0;

    Vector3 p0 = input[input.size() - 1];
    if (p0.y < offset) state += 2;

    for (size_t i = 0; i < input.size(); i++)
    {
        state = state >> 1;

        Vector3 p1 = input[i];
        if (p1.y < offset) state += 2;

        switch (state)
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
                auto newPoint = (p0 + diff * (offset - p0.y) / diff.y);
                newPoint.y = offset;
                output.push_back(newPoint);
                break;
            };
            /* 01 */ case 1:
            {
                auto diff = p1 - p0;
                auto newPoint = (p0 + diff * (offset - p0.y) / diff.y);
                newPoint.y = offset;
                output.push_back(newPoint);
                break;
            };
            default: break;
        }

        p0 = p1;
    }
}