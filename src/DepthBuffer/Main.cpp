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
        //     float zFar = 0;
        //     float zClose = -10;
        //     {
        //         Vector3 p0 = { 100, 350, zFar };
        //         Vector3 p1 = { 250,  50, zClose };
        //         for (size_t i = 0; i < 7; i++)
        //         {
        //             p0.y += 1;
        //             p1.y += 1;
        //             bitmap->DrawLine3(p0, p1, RED);
        //         }
        //     }
        //     {
        //         Vector3 p0 = { 150,  50, zFar };
        //         Vector3 p1 = { 300, 350, zClose };
        //         for (size_t i = 0; i < 7; i++)
        //         {
        //             p0.y += 1;
        //             p1.y += 1;
        //             bitmap->DrawLine3(p0, p1, GREEN);
        //         }
        //     }
        //     {
        //         Vector3 p0 = { 350, 250, zFar };
        //         Vector3 p1 = {  50, 250, zClose };
        //         for (size_t i = 0; i < 7; i++)
        //         {
        //             p0.y += 1;
        //             p1.y += 1;
        //             bitmap->DrawLine3(p0, p1, BLUE);
        //         }
        //     }
        // }

        // LINES
        {
            float zFar = 0;
            float zClose = -10;
            {
                Vector3 p0 = { 100, 350, zFar };
                Vector3 p1 = { 250,  50, zClose };
                for (size_t i = 0; i < 7; i++)
                {
                    p0.y += 1;
                    p1.y += 1;
                    bitmap->DrawLine3(p0, p1, RED);
                }
            }
            {
                Vector3 p0 = { 150,  50, zFar };
                Vector3 p1 = { 300, 350, zClose };
                for (size_t i = 0; i < 7; i++)
                {
                    p0.y += 1;
                    p1.y += 1;
                    bitmap->DrawLine3(p0, p1, GREEN);
                }
            }
            {
                Vector3 p0 = { 350, 250, zFar };
                Vector3 p1 = {  50, 250, zClose };
                for (size_t i = 0; i < 7; i++)
                {
                    p0.y += 1;
                    p1.y += 1;
                    bitmap->DrawLine3(p0, p1, BLUE);
                }
            }
        }

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

        // // 3 TRIANGLES
        // {
        //     auto time = ((float)clock() / 4000) - 1;
        //     float zFar = 0;
        //     float zClose = -1;
        //     {
        //         Vector3 p0 = { -0.2f,  0.6f,  zFar };
        //         Vector3 p1 = {  0.0f,  0.6f,  zFar };
        //         Vector3 p2 = {  0.4f, -0.6f,  zClose };
        //         bitmap->DrawTriangle2(p0, p1, p2, RED);
        //     }
        //     {
        //         Vector3 p0 = {  0.6f, -0.2f,  zFar };
        //         Vector3 p1 = {  0.6f, -0.4f,  zFar };
        //         Vector3 p2 = { -0.8f, -0.2f,  zClose };
        //         bitmap->DrawTriangle2(p0, p1, p2, GREEN);
        //     }
        //     {
        //         Vector3 p0 = { -0.4f, -0.4f,  zFar };
        //         Vector3 p1 = { -0.6f, -0.4f,  zFar };
        //         Vector3 p2 = {  0.2f,  0.6f,  zClose };
        //         bitmap->DrawTriangle2(p0, p1, p2, BLUE);
        //     }
        // }

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