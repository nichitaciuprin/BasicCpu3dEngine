unique_ptr<BitmapWindow2> window = nullptr;

char frame[1024];
// uint64_t targetAddr;

void InitClientWindow()
{
    // targetAddr = NetCreateAddr(127, 0, 0, 1, 27015);

    auto width = 32;
    auto height = 32;
    auto scale = 16;
    auto size = 32*16;

    window = make_unique<BitmapWindow2>(700, 100, size, size);
}
bool ClientWindowClosed()
{
    return !window->Exists();
}
void UpdateClientWindow()
{
    window->Update();
}
void RenderClientWindow()
{
    bool messageReceived = NetRecvFrame(frame);
    if (!messageReceived) return;

    auto width = 32;
    auto height = 32;
    auto scale = 16;
    auto size = 32*16;

    window->SetPixelsScaled2((uint8_t*)frame, width, height, scale);
}
void SendInput()
{
    NetInput netInput = {};
    netInput.w = window->KeyDown_W();
    netInput.a = window->KeyDown_A();
    netInput.s = window->KeyDown_S();
    netInput.d = window->KeyDown_D();
    NetSendInput(&netInput);
}
void RecvFrame()
{
    NetRecvFrame(frame);

    auto width = 32;
    auto height = 32;
    auto scale = 16;
    auto size = 32*16;

    window->SetPixelsScaled2((uint8_t*)frame, width, height, scale);
}
