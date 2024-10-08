Camera camera;
unique_ptr<Bitmap> bitmap;
unique_ptr<BitmapWindow2> window;

void Draw(Bitmap& bitmap, Camera camera, long time)
{
    bitmap.Fill(BLACK);

    auto view = MatrixView(&camera);

    {
        Vector3 p0 = { -1, 0,  2 }; p0 *= view;
        Vector3 p1 = { -1, 0, 95 }; p1 *= view;
        Vector3 p2 = {  1, 0, 95 }; p2 *= view;
        Vector3 p3 = {  1, 0,  2 }; p3 *= view;
        bitmap.DrawPoligon1(p0, p1, p2, p3, WHITE);
    }
    {
        float size = 5;
        Vector3 p0 = { -size, 0, -size + 100 }; p0 *= view;
        Vector3 p1 = { -size, 0,  size + 100 }; p1 *= view;
        Vector3 p2 = {  size, 0,  size + 100 }; p2 *= view;
        Vector3 p3 = {  size, 0, -size + 100 }; p3 *= view;
        bitmap.DrawPoligon1(p0, p1, p2, p3, WHITE);
    }

    // TODO fix this
    // bitmap->ApplyBlackWhiteColorDepth();

    {
        auto time2 = (float)time / 600;
        Vector3 position = { 0, 0.5f, 100 };
        Vector3 rotation = { 0, time2, 0 };
        Vector3 scale = { 1, 1, 1 };
        auto world = MatrixWorld(position, rotation, scale);
        bitmap.DrawCubeColored(world * view);
    }
    {
        auto time2 = (float)time / 300;
        Vector3 position = { 0, 1.5f, 100 };
        Vector3 rotation = { 0, time2, 0 };
        Vector3 scale = { 1, 1, 1 };
        auto world = MatrixWorld(position, rotation, scale);
        bitmap.DrawCubeColored(world * view);
    }
}
void InitGame()
{
    camera = { 0, 1, 95 };
}
void InitGameWindow()
{
    auto width = 32;
    auto height = 32;
    auto scale = 16;
    auto size = 32*16;

    bitmap = make_unique<Bitmap>(width, height);
    window = make_unique<BitmapWindow2>(0, 100, size, size);
}
bool GameWindowClosed()
{
    return !window->Exists();
}
void UpdateGameWindow()
{
    window->Update();
}
void RenderGame()
{
    Draw(*bitmap, camera, clock());

    window->SetPixelsScaled(bitmap->pixels.data(), 32, 32, 16);

    char buffer[1024];

    for (int i = 0; i < 1024; i++)
        buffer[i] = PixelToLightValue(bitmap->pixels[i]);

    NetSendFrame(buffer);
}
void UpdateGame()
{
    bool w, a, s, d;
    bool up, left, down, right;
    bool e, q;

    left = window->KeyDown_LEFT();
    up = window->KeyDown_UP();
    down = window->KeyDown_DOWN();
    right = window->KeyDown_RIGHT();
    e = window->KeyDown_E();
    q = window->KeyDown_Q();

    NetRecvInput(&w, &a, &s, &d);

    UpdateCameraRotation(&camera, 0.023f, left, up, down, right);
    UpdateCameraPosition(&camera, 0.008f, w, a, s, d, e, q);
}