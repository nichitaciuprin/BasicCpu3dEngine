#include "../src/Base/Base.h"

void main2()
{
    auto width = 400;
    auto height = 400;

    auto bitmap = make_unique<Bitmap>(width, height);
    auto window = make_unique<BitmapWindow>(700, 100, width, height);

    Camera camera = { 0, 1, 0 };

    while (window->Exists())
    {
        CheckFPS();
        bitmap->Fill(BLACK);

        auto view = MatrixView(&camera);

        UpdateCameraRotation
        (
            &camera, 0.003f,
            window->keydown_VK_LEFT,
            window->keydown_VK_UP,
            window->keydown_VK_DOWN,
            window->keydown_VK_RIGHT
        );

        UpdateCameraPosition
        (
            &camera, 0.002f,
            window->keydown_W, window->keydown_A, window->keydown_S, window->keydown_D,
            window->keydown_E, window->keydown_Q
        );

        {
            Vector3 p0 = { -1, 0,  2 }; p0 *= view;
            Vector3 p1 = { -1, 0, 95 }; p1 *= view;
            Vector3 p2 = {  1, 0, 95 }; p2 *= view;
            Vector3 p3 = {  1, 0,  2 }; p3 *= view;
            bitmap->DrawPoligon(p0, p1, p2, p3, WHITE);
        }
        {
            float size = 5;
            Vector3 p0 = { -size, 0, -size + 100 }; p0 *= view;
            Vector3 p1 = { -size, 0,  size + 100 }; p1 *= view;
            Vector3 p2 = {  size, 0,  size + 100 }; p2 *= view;
            Vector3 p3 = {  size, 0, -size + 100 }; p3 *= view;
            bitmap->DrawPoligon(p0, p1, p2, p3, WHITE);
        }

        bitmap->ApplyBlackWhiteColorDepth();

        {
            auto time = (float)clock() / 40;
            Vector3 position = { 0, 0.5f, 100 };
            Vector3 rotation = { 0, time, 0 };
            Vector3 scale = { 1, 1, 1 };
            auto world = MatrixWorld(position, rotation, scale);
            bitmap->DrawCube4(world * view);
        }
        {
            auto time = (float)clock() / 20;
            Vector3 position = { 0, 1.5f, 100 };
            Vector3 rotation = { 0, time, 0 };
            Vector3 scale = { 1, 1, 1 };
            auto world = MatrixWorld(position, rotation, scale);
            bitmap->DrawCube4(world * view);
        }

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