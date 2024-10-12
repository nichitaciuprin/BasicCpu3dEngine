struct Player
{
    uint64_t id;
    float timer;
    Camera camera;
};

unique_ptr<Bitmap> bitmapNet;
vector<Player> players;

void DestroyPlayers(float deltaTime)
{
    for (int i = players.size() - 1; i >= 0; i--)
    {
        auto& player = players[i];
        player.timer -= deltaTime;
        if (player.timer <= 0)
            erase_back(players, i);
    }
}
void UpdatePlayer(uint64_t playerAddr, NetInput& recvInput)
{
    for (size_t i = 0; i < players.size(); i++)
    {
        auto& player = players[i];

        // cout << player.id << ":" << recvInput.id << endl;

        if (player.id == playerAddr)
        {
            player.timer == 10;
            bool w = recvInput.w;
            bool a = recvInput.a;
            bool s = recvInput.s;
            bool d = recvInput.d;
            bool q = false;
            bool e = false;
            UpdateCameraPosition(&player.camera, 0.008f, w, a, s, d, q, e);
            return;
        }
    }

    if (players.size() == 4) return;

    // cout << recvInput.id << endl;

    Player player = {};
    player.id = playerAddr;
    player.timer = 10;
    players.push_back(player);
}

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
    auto scale = 16;
    auto size1 = 32;
    auto size2 = 32*scale;

    bitmapNet = make_unique<Bitmap>(size1, size1);

    players = vector<Player>();
    players.reserve(4);
}
void InitGameWindow()
{
    auto scale = 16;
    auto size1 = 32;
    auto size2 = 32*scale;

    // bitmapNet = make_unique<Bitmap>(size1, size1);
    // bitmap = make_unique<Bitmap>(size2, size2);
    // window = make_unique<BitmapWindow2>(0, 100, size2, size2);
}
// bool GameWindowClosed()
// {
//     return !window->Exists();
// }
// void UpdateGameWindow()
// {
//     window->Update();
// }

void RenderGame()
{
    for (auto& i : players)
    {
        Draw(*bitmapNet, i.camera, clock());

        char buffer[1024];

        for (int i = 0; i < 1024; i++)
            buffer[i] = PixelToLightValue(bitmapNet->pixels[i]);

        // cout << i.id << endl;

        NetSendFrame(&i.id, buffer);
    }
}
void UpdateGame(float deltaTime)
{
    bool w, a, s, d;
    bool up, left, down, right;
    bool e, q;

    // left = window->KeyDown_LEFT();
    // up = window->KeyDown_UP();
    // down = window->KeyDown_DOWN();
    // right = window->KeyDown_RIGHT();
    // e = window->KeyDown_E();
    // q = window->KeyDown_Q();

    DestroyPlayers(deltaTime);

    uint64_t addr;
    NetInput netInput;

    while (NetRecvInput(&addr, &netInput))
    {
        // cout << "123" << endl;

        // UpdateCameraRotation(&camera, 0.023f, left, up, down, right);
        // UpdateCameraPosition(&camera, 0.008f, w, a, s, d, e, q);

        UpdatePlayer(addr, netInput);
    }
}

Camera testCamera;
unique_ptr<Bitmap> testBitmap;
unique_ptr<BitmapWindow2> testWindow;
bool TestRenderCalled = false;
void TestRender()
{
    if (!TestRenderCalled)
    {
        TestRenderCalled = true;

        testCamera = { 0, 1, 95 };

        auto size = 32*16;

        testBitmap = make_unique<Bitmap>(size, size);
        testWindow = make_unique<BitmapWindow2>(0, 0, 512, 512);

        return;
    }

    Draw(*testBitmap, testCamera, clock());
    testWindow->SetPixels(testBitmap->pixels.data(), 32*16, 32*16);
    testWindow->Update();
}