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
            &camera, 0.0001f,
            window->keydown_W, window->keydown_A, window->keydown_S, window->keydown_D,
            window->keydown_E, window->keydown_Q
        );

        {
            auto time = (float)clock() / 40;
            Vector3 position = { 0, 0, 2 };
            Vector3 rotation = { 0, time, 0 };
            Vector3 scale = { 1, 1, 1 };
            auto world = MatrixWorld(position, rotation, scale);
            bitmap->DrawCube4(world * view);
        }
        {
            Vector3 position = { 0.5f, 0.5f, 2 };
            Vector3 rotation = { 0, 0, 0 };
            Vector3 scale = { 1, 1, 1 };
            auto world = MatrixWorld(position, rotation, scale);
            bitmap->DrawCube4(world * view);
        }

        // bitmap->DrawBorder(GREEN);
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