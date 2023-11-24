#include "../Base/Base.h"

void main2()
{
    auto width = 400;
    auto height = 400;

    auto bitmap = make_unique<Bitmap>(width, height);

    BitmapWindow1::Create(700, 100, width, height);

    while (BitmapWindow1::Exists())
    {
        CheckFPS();
        bitmap->Fill(BLACK);

        // // LINES
        // {
        //     float zFar = 10;
        //     float zClose = 9;
        //     {
        //         Vector3 p0 = { -1,  3,  zFar };    p0 /= 10;
        //         Vector3 p1 = {  2, -3,  zClose };  p1 /= 10;
        //         bitmap->DrawLine1(p0, p1, RED);
        //     }
        //     {
        //         Vector3 p0 = {  3, -1,  zFar };    p0 /= 10;
        //         Vector3 p1 = { -3, -1,  zClose };  p1 /= 10;
        //         bitmap->DrawLine1(p0, p1, GREEN);
        //     }
        //     {
        //         Vector3 p0 = { -2, -2,  zFar };    p0 /= 10;
        //         Vector3 p1 = {  1,  3,  zClose };  p1 /= 10;
        //         bitmap->DrawLine1(p0, p1, BLUE);
        //     }
        // }

        // // 2 TRIANGLES
        // // RED must overlap
        // {
        //     float zFar = 10;
        //     float zClose = 5;
        //     {
        //         Vector3 p0 = { -1,  3,  zFar };
        //         Vector3 p1 = {  1,  3,  zFar };
        //         Vector3 p2 = {  0, -3,  zFar };
        //         p0 /= 10;
        //         p1 /= 10;
        //         p2 /= 10;
        //         bitmap->DrawTriangle1(p0, p1, p2, RED);
        //     }
        //     {
        //         Vector3 p0 = { -3, -1,  zFar + 1 };
        //         Vector3 p1 = { -3,  1,  zFar + 1 };
        //         Vector3 p2 = {  3,  0,  zFar + 1 };
        //         p0 /= 10;
        //         p1 /= 10;
        //         p2 /= 10;
        //         bitmap->DrawTriangle1(p0, p1, p2, GREEN);
        //     }
        // }


        // 3 TRIANGLES
        {
            auto time = (float)clock() / 4000;
            float zFar = 1;
            float zClose = 0;
            {
                Vector3 p0 = { -0.2f,  0.6f,  zFar };
                Vector3 p1 = {  0.0f,  0.6f,  zFar };
                Vector3 p2 = {  0.4f, -0.6f,  zClose };
                bitmap->DrawTriangle2(p0, p1, p2, RED);
            }
            {
                Vector3 p0 = {  0.6f, -0.2f,  zFar };
                Vector3 p1 = {  0.6f, -0.4f,  zFar };
                Vector3 p2 = { -0.8f, -0.2f,  zClose };
                bitmap->DrawTriangle2(p0, p1, p2, GREEN);
            }
            {
                Vector3 p0 = { -0.4f, -0.4f,  zFar };
                Vector3 p1 = { -0.6f, -0.4f,  zFar };
                Vector3 p2 = {  0.2f,  0.6f,  zClose };
                bitmap->DrawTriangle2(p0, p1, p2, BLUE);
            }
        }


        bitmap->DrawBorder(GREEN);
        BitmapWindow1::SetPixels(bitmap);
        BitmapWindow1::Update();
    }
}

int main()
{
    try
    {
        main2();
    }
    catch (const exception& e)
    {
        cerr << e.what() << endl;
    }

    return 0;
}