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
    InputState GetInputState();

private:
    BitmapWindowPrivate* instance;
};