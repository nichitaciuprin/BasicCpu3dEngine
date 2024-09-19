struct InputState
{
    bool w, a, s, d;
    bool left, up, down, right;
    bool e, q;
};

class BitmapWindowPrivate;

class BitmapWindow
{
public:
    BitmapWindow(int x, int y, int clientWidth, int clientHeight);
    ~BitmapWindow();

    bool Exists() const;
    void Update();

    void SetPixels(uint32_t* pixels, int width, int height);
    void SetPixelsScaled(uint32_t* pixels, int width, int height, int scale);

    bool KeyDown_W();
    bool KeyDown_A();
    bool KeyDown_S();
    bool KeyDown_D();
    bool KeyDown_E();
    bool KeyDown_Q();
    bool KeyDown_UP();
    bool KeyDown_LEFT();
    bool KeyDown_DOWN();
    bool KeyDown_RIGHT();

private:
    BitmapWindowPrivate* instance;
};