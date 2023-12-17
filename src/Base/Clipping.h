#pragma once

void ClipPoligonLeft(vector<Vector3>& input, vector<Vector3>& output)
{
    float offset = -1;

    Vector3& p0 = input[input.size() - 1];
    bool c0 = p0.x < offset;

    for (size_t i = 0; i < input.size(); i++)
    {
        Vector3& p1 = input[i];
        bool c1 = p1.x < offset;

        int state = 0;
        if (c1) state += 1;
        if (c0) state += 2;

        switch (state)
        {
            /* 00 */ case 0: {  output.push_back(p0); break; };
            /* 01 */ case 1: {  output.push_back(p0); output.push_back({ offset, p1.y + (p1.y - p0.y) * (offset - p1.x) / (p1.x - p0.x) }); break; };
            /* 10 */ case 2: {                        output.push_back({ offset, p0.y + (p0.y - p1.y) * (offset - p0.x) / (p0.x - p1.x) }); break; };
            default: break;
        }

        p0 = p1;
        c0 = c1;
    }
}
void ClipPoligonRight(vector<Vector3>& input, vector<Vector3>& output)
{
    cout << "CLIP" << endl;
    float offset = 1;

    Vector3& p0 = input[input.size() - 1];
    bool c0 = p0.x > offset;

    for (size_t i = 0; i < input.size(); i++)
    {
        Vector3& p1 = input[i];
        bool c1 = p1.x > offset;

        int state = 0;
        if (c1) state += 1;
        if (c0) state += 2;
        // Vector3Print(p0);
        Vector3Print(p1);

        switch (state)
        {
            /* 00 */ case 0:
            {
                cout << 0 << endl;
                output.push_back(p0); // Vector3Print(p0);
                break;
            };
            /* 01 */ case 1:
            {
                cout << 1 << endl;
                output.push_back(p0); // Vector3Print(p0);
                Vector3 newPoint = { offset, p0.y + (p1.y - p0.y) * (offset - p0.x) / (p1.x - p0.x) };
                output.push_back(newPoint); // Vector3Print(newPoint);
                break;
            };
            /* 10 */ case 2:
            {
                cout << 2 << endl;
                Vector3 newPoint = { offset, p1.y + (p0.y - p1.y) * (offset - p1.x) / (p0.x - p1.x) };
                output.push_back(newPoint); // Vector3Print(p0);
                break;
            };
            default: cout << 3 << endl; break;
        }

        p0 = p1;
        c0 = c1;
    }
}

// void ClipPoligonRight(vector<Vector3>& input, vector<Vector3>& output)
// {
//     int pointsDone = 0;
//     Vector3& p0 = input[input.size() - 1];
//     if (p0.x <= 1) { goto INCLUDE; }
//     else           { goto EXCLUDE; }

//     INCLUDE:
//     {
//         do
//         {
//             output.push_back(p0);
//             Vector3& p1 = input[pointsDone];
//             pointsDone++;
//             if (p1.x > 1)
//             {
//                 Vector3 newPoint = { 1, p0.y + (p1.y - p0.y) * (1 - p0.x) / (p1.x - p0.x) };
//                 output.push_back(newPoint);
//                 p0 = p1;
//                 goto EXCLUDE;
//             }
//             else
//             {
//                 p0 = p1;
//             }
//         }
//         while (pointsDone != input.size());
//         return;
//     }

//     EXCLUDE:
//     {
//         do
//         {
//             Vector3& p1 = input[pointsDone];
//             pointsDone++;
//             if (p1.x <= 1)
//             {
//                 Vector3 newPoint = { 1, p1.y + (p0.y - p1.y) * (1 - p1.x) / (p0.x - p1.x) };
//                 output.push_back(newPoint);
//                 p0 = p1;
//                 goto INCLUDE;
//             }
//             else
//             {
//                 p0 = p1;
//             }
//         }
//         while (pointsDone != input.size());
//         return;
//     }
// }
void ClipPoligon(vector<Vector3>& in, vector<Vector3>& out)
{
    //              ClipPoligonLeft   (in, out); swap(in, out);
    // out.clear(); ClipPoligonTop    (in, out); swap(in, out);
    // out.clear(); ClipPoligonRight  (in, out); swap(in, out);
    // out.clear(); ClipPoligonBottom (in, out);

    cout << "---0---" << endl; for (auto& point : in) Vector3Print(point);
    ClipPoligonLeft(in, out); in.clear();
    cout << "---1---" << endl; for (auto& point : out) Vector3Print(point);
    ClipPoligonRight(out, in); out.clear();
    cout << "---2---" << endl; for (auto& point : in) Vector3Print(point);
    for (auto& point : in)
        out.push_back(point);


    // ClipPoligonLeft(in, out);
    // ClipPoligonRight(v0, v1);
    // in.clear();

    // ClipPoligonLeft(in, out);
    // ClipPoligonRight(in, out);
    // cout << "---0---" << endl; for (auto& point : out) Vector3Print(point);
}
void ClipTriangle(Vector3 p0, Vector3 p1, Vector3 p2, vector<Vector3>& points)
{
    auto input = vector<Vector3> { p0, p1, p2 };
    ClipPoligon(input, points);
}
// void ClipTriangleRight(Vector3& p0, Vector3& p1, Vector3& p2, Vector3& p3, int& outCode)
// {
//     float offset = 1;

//     int state = 0;

//     if (p2.x > offset) state += 1;
//     if (p1.x > offset) state += 2;
//     if (p0.x > offset) state += 4;

//     switch (state)
//     {
//         /* 000 */ case 0: { outCode = 0; return; };
//         /* 111 */ case 7: { outCode = 3; return; };
//         default: break;
//     }

//     Vector3 v0, v1, v2;

//     switch (state)
//     {
//         /* 100 */ case 4: { v0 = p0; v1 = p1; v2 = p2; goto POINT_1; };
//         /* 010 */ case 2: { v0 = p1; v1 = p2; v2 = p0; goto POINT_1; };
//         /* 001 */ case 1: { v0 = p2; v1 = p0; v2 = p1; goto POINT_1; };

//         /* 011 */ case 3: { v0 = p0; v1 = p1; v2 = p2; goto POINT_2; };
//         /* 101 */ case 5: { v0 = p1; v1 = p2; v2 = p0; goto POINT_2; };
//         /* 110 */ case 6: { v0 = p2; v1 = p0; v2 = p1; goto POINT_2; };

//         default: __assume(0);
//     }

//     POINT_1:
//     {
//         outCode = 1;
//         p0 = { offset, v0.y + (v2.y - v0.y) * (offset - v0.x) / (v2.x - v0.x) };
//         p1 = { offset, v0.y + (v1.y - v0.y) * (offset - v0.x) / (v1.x - v0.x) };
//         p2 = v1;
//         p3 = v2;
//         return;
//     }

//     POINT_2:
//     {
//         outCode = 2;
//         p0 = v0;
//         p1 = { offset, v1.y + (v0.y - v1.y) * (offset - v1.x) / (v1.x - v0.x) };
//         p2 = { offset, v2.y + (v0.y - v2.y) * (offset - v2.x) / (v2.x - v0.x) };
//         return;
//     }
// }
// void ClipTriangleLeft(Vector3& p0, Vector3& p1, Vector3& p2, Vector3& p3, int& outCode)
// {
//     float offset = -1;

//     int state = 0;

//     if (p2.x < offset) state += 1;
//     if (p1.x < offset) state += 2;
//     if (p0.x < offset) state += 4;

//     switch (state)
//     {
//         /* 000 */ case 0: { outCode = 0; return; };
//         /* 111 */ case 7: { outCode = 3; return; };
//         default: break;
//     }

//     Vector3 v0, v1, v2;

//     switch (state)
//     {
//         /* 100 */ case 4: { v0 = p0; v1 = p1; v2 = p2; goto POINT_1; };
//         /* 010 */ case 2: { v0 = p1; v1 = p2; v2 = p0; goto POINT_1; };
//         /* 001 */ case 1: { v0 = p2; v1 = p0; v2 = p1; goto POINT_1; };

//         /* 011 */ case 3: { v0 = p0; v1 = p1; v2 = p2; goto POINT_2; };
//         /* 101 */ case 5: { v0 = p1; v1 = p2; v2 = p0; goto POINT_2; };
//         /* 110 */ case 6: { v0 = p2; v1 = p0; v2 = p1; goto POINT_2; };

//         default: __assume(0);
//     }

//     POINT_1:
//     {
//         outCode = 1;
//         p0 = { offset, v0.y + (v2.y - v0.y) * (offset - v0.x) / (v2.x - v0.x) };
//         p1 = { offset, v0.y + (v1.y - v0.y) * (offset - v0.x) / (v1.x - v0.x) };
//         p2 = v1;
//         p3 = v2;
//         return;
//     }

//     POINT_2:
//     {
//         outCode = 2;
//         p0 = v0;
//         p1 = { offset, v1.y + (v0.y - v1.y) * (offset - v1.x) / (v1.x - v0.x) };
//         p2 = { offset, v2.y + (v0.y - v2.y) * (offset - v2.x) / (v2.x - v0.x) };
//         return;
//     }
// }
