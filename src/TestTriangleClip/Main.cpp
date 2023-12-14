#include "../Base/Base.h"

void main2()
{
    auto width = 400;
    auto height = 400;

    auto bitmap = make_unique<Bitmap>(width, height);
    auto window = make_unique<BitmapWindow>(700, 100, width, height);

    Camera camera = { 0, 0, 0 };

    while (window->Exists())
    {
        CheckFPS();
        bitmap->Fill(BLACK);

        UpdateCameraRotation
        (
            &camera, 0.001f,
            window->keydown_VK_LEFT,
            window->keydown_VK_UP,
            window->keydown_VK_DOWN,
            window->keydown_VK_RIGHT
        );

        UpdateCameraPosition
        (
            &camera, 0.0002f,
            window->keydown_W, window->keydown_A, window->keydown_S, window->keydown_D,
            window->keydown_E, window->keydown_Q
        );

        auto view = MatrixView(&camera);

        {
            Vector3 p0 = { -1,  0,  1 }; p0 *= view;
            Vector3 p1 = {  1,  1,  1 }; p1 *= view;
            Vector3 p2 = {  1, -1,  1 }; p2 *= view;
            // Vector3 p2 = { -1,  0,  1 }; p2 *= view;
            // Vector3 p0 = {  1,  1,  1 }; p0 *= view;
            // Vector3 p1 = {  1, -1,  1 }; p1 *= view;
            // Vector3 p1 = { -1,  0,  1 }; p1 *= view;
            // Vector3 p2 = {  1,  1,  1 }; p2 *= view;
            // Vector3 p0 = {  1, -1,  1 }; p0 *= view;
            bitmap->DrawTriangle1(p0, p1, p2, WHITE);
        }
        // {
        //     Vector3 p0 = {  1,  0,  1 }; p0 *= view;
        //     Vector3 p1 = { -1, -1,  1 }; p1 *= view;
        //     Vector3 p2 = { -1,  1,  1 }; p2 *= view;
        //     // Vector3 p2 = {  1,  0,  1 }; p2 *= view;
        //     // Vector3 p0 = { -1, -1,  1 }; p0 *= view;
        //     // Vector3 p1 = { -1,  1,  1 }; p1 *= view;
        //     // Vector3 p1 = {  1,  0,  1 }; p1 *= view;
        //     // Vector3 p2 = { -1, -1,  1 }; p2 *= view;
        //     // Vector3 p0 = { -1,  1,  1 }; p0 *= view;
        //     bitmap->DrawTriangle1(p0, p1, p2, WHITE);
        // }

        window->SetPixels(bitmap);
        window->Update();
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