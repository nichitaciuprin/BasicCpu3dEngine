unique_ptr<BitmapWindow2> window = nullptr;

char frame[1024];

void InitClientWindow()
{
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
    NetRecvFrame(frame);

    auto width = 32;
    auto height = 32;
    auto scale = 16;
    auto size = 32*16;

    window->SetPixelsScaled2((uint8_t*)frame, width, height, scale);
}
void SendInput()
{
    bool w = window->KeyDown_W();
    bool a = window->KeyDown_A();
    bool s = window->KeyDown_S();
    bool d = window->KeyDown_D();
    NetSendInput(w, a, s, d);
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
