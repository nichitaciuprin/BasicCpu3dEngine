struct InputState
{
    bool w, a, s, d;
    bool left, up, down, right;
    bool e, q;
};

class BitmapWindow;

class BitmapWindow2
{
public:
    BitmapWindow2(int x, int y, int clientWidth, int clientHeight);
    ~BitmapWindow2();

    bool Exists() const;
    void Update();

    // void SetPixel(int x, int y, uint32_t pixel);
    void SetPixels(uint32_t* pixels, int width, int height);
    // void SetPixelsScaled(uint32_t* pixels, int width, int height, int scale);
    // int GetClientWidth() const;
    // int GetClientHeight() const;
    InputState GetInputState();

private:
    BitmapWindow* instance;
};