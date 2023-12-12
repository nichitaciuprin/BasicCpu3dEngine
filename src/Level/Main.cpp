#include "../Base/Base.h"

// #include <iomanip>

void main2()
{
    // auto byte = (int)(255.1f);
    // uint32_t pixel = 0;
    // pixel += byte; pixel = pixel << 8;
    // pixel += byte; pixel = pixel << 8;
    // pixel += byte; pixel = pixel << 8;
    // pixel += byte;
    // std::cout << std::setfill('0') << std::setw(8) << std::hex << pixel << '\n';

    // return;

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
            // auto time = (float)clock() / 40;
            // Vector3 position = { 0, 0, 2 };
            // Vector3 rotation = { 0, time, 0 };
            // Vector3 scale = { 1, 1, 1 };
            // auto world = MatrixWorld(position, rotation, scale);
            // bitmap->DrawCube4(world * view);
        }
        {
            // Vector3 position = { 0.5f, 0.5f, 2 };
            // Vector3 rotation = { 0, 0, 0 };
            // Vector3 scale = { 1, 1, 1 };
            // auto world = MatrixWorld(position, rotation, scale);
            // bitmap->DrawCube4(world * view);
        }
        {
            Vector3 p0 = { -1, -1,   1 }; p0 *= view;
            Vector3 p1 = { -1, -1, 100 }; p1 *= view;
            Vector3 p2 = {  1, -1, 100 }; p2 *= view;
            Vector3 p3 = {  1, -1,   1 }; p3 *= view;
            bitmap->DrawPoligon(p0, p1, p2, p3, WHITE);

            // Vector3 p0 = { -1, 0, 3 };
            // Vector3 p1 = { -1, 1, 3 };
            // Vector3 p2 = {  1, 1, 3 };
            // bitmap->DrawTriangle0(p0, p1, p2, MAGENTA,);
        }

        // bitmap->DrawBorder(GREEN);
        bitmap->ApplyBlackWhiteColorDepth();
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