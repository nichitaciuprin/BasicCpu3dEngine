#include <Std.h>
#include <StdExt.h>
#include <SysHelper.h>

#include <X11/Xlib.h>

typedef struct BitmapWindow
{
    bool keydown_W;
    bool keydown_A;
    bool keydown_S;
    bool keydown_D;
    bool keydown_E;
    bool keydown_UP;
    bool keydown_Q;
    bool keydown_LEFT;
    bool keydown_DOWN;
    bool keydown_RIGHT;

    int  _width;
    int  _height;

    XImage* image;
    uint32_t* _pixels;
    Atom wm_delete_window;
    bool windowClosed = false;
    Display* display;
    Window window;
}
BitmapWindow;

BitmapWindow* BitmapWindow_Create(int x, int y, int clientWidth, int clientHeight)
{
    BitmapWindow* instance = (BitmapWindow*)malloc(sizeof(BitmapWindow));

    instance->windowClosed = false;

    instance->_width = clientWidth;
    instance->_height = clientHeight;

    instance->keydown_W = false;
    instance->keydown_A = false;
    instance->keydown_S = false;
    instance->keydown_D = false;
    instance->keydown_E = false;
    instance->keydown_Q = false;
    instance->keydown_UP = false;
    instance->keydown_LEFT = false;
    instance->keydown_DOWN = false;
    instance->keydown_RIGHT = false;

    instance->display = XOpenDisplay(NULL);

    int screen = DefaultScreen(instance->display);
    Window root = DefaultRootWindow(instance->display);

    instance->window = XCreateSimpleWindow(instance->display, root, 0, 0, clientWidth, clientHeight, 0, 0, 0xffffffff);

    XSelectInput(instance->display, instance->window, ExposureMask | KeyPressMask);
    XMapWindow(instance->display, instance->window);

    instance->wm_delete_window = XInternAtom(instance->display, "WM_DELETE_WINDOW", False);

    XSetWMProtocols(instance->display, instance->window, &instance->wm_delete_window, 1);

    Visual* visual = DefaultVisual(instance->display, screen);
    int depth = DefaultDepth(instance->display, screen);

    instance->_pixels = (uint32_t*)malloc(clientWidth * clientHeight * 4);

    instance->image = XCreateImage
    (
        instance->display, visual, depth, ZPixmap, 0,
        (char*)instance->_pixels, clientWidth, clientHeight, 32, 0
    );

    return instance;
}
bool BitmapWindow_Exists(BitmapWindow* instance)
{
    return !instance->windowClosed;
}
void BitmapWindow_Destroy(BitmapWindow* instance)
{
    if (!BitmapWindow_Exists(instance)) return;

    instance->windowClosed = true;
    XDestroyWindow(instance->display, instance->window);
}
void BitmapWindow_Update(BitmapWindow* instance)
{
    if (!BitmapWindow_Exists(instance)) return;

    XEvent event;

    int messageCount = XPending(instance->display);

    for (size_t i = 0; i < messageCount; i++)
    {
        XNextEvent(instance->display, &event);

        switch (event.type)
        {
            case ClientMessage:
            {
                printf("!\n");

                if (event.xclient.data.l[0] == instance->wm_delete_window)
                {
                    instance->windowClosed = true;
                    XDestroyWindow(instance->display, instance->window);
                    // XDestroyWindow(event.xclient.display, event.xclient.window);
                }
            }
            break;

            case Expose:
            {
                // long time1 = GetTime();

                // for (int x = 0; x < width; x++)
                // for (int y = 0; y < height; y++)
                // {
                //     uint8_t r = (time1 / 10) % 255;
                //     uint8_t g = (time1 / 10) % 255;
                //     uint8_t b = 0;
                //     uint8_t a = 0;

                //     uint32_t pixel = 0;

                //     pixel += a; pixel <<= 8;
                //     pixel += r; pixel <<= 8;
                //     pixel += g; pixel <<= 8;
                //     pixel += b;

                //     pixels[y * width + x] = pixel;
                // }

                // TODO should be cached?
                int screen = DefaultScreen(instance->display);
                GC defaultGc = DefaultGC(instance->display, screen);

                XPutImage(instance->display, instance->window, defaultGc, instance->image, 0, 0, 0, 0, instance->_width, instance->_height);

                XEvent se;
                se.type = Expose;
                se.xexpose.type = Expose;
                se.xexpose.serial = 0;
                se.xexpose.send_event = 1;
                se.xexpose.window = instance->window;
                se.xexpose.x = 0;
                se.xexpose.y = 0;
                se.xexpose.width = instance->_width;
                se.xexpose.height = instance->_height;
                se.xexpose.count = 0;
                XSendEvent(instance->display, instance->window, false, 0, &se);
            }
            break;

            case KeyPress:
            {
                uint32_t keyKode = event.xkey.keycode;

                uint32_t esc   = 0x09;
                uint32_t w     = 0x19;
                uint32_t a     = 0x26;
                uint32_t s     = 0x27;
                uint32_t d     = 0x28;
                uint32_t up    = 0x6f;
                uint32_t left  = 0x71;
                uint32_t down  = 0x74;
                uint32_t right = 0x72;
                uint32_t q     = 0x18;
                uint32_t e     = 0x1a;

                printf("keyKode:%x\n", keyKode);

                // if (keyKode == esc)
                //     quit = 1;
            }
            break;

            default:
                printf("missed event %i\n", event.type);
                break;
        }
    }
}
void BitmapWindow_SetPixel(BitmapWindow* instance, int x, int y, uint32_t pixel)
{
    if (!BitmapWindow_Exists(instance)) return;

    instance->_pixels[x + y * instance->_width] = pixel;
}
void BitmapWindow_SetPixels(BitmapWindow* instance, uint32_t* pixels, int width, int height)
{
    if (!BitmapWindow_Exists(instance)) return;

    memcpy(instance->_pixels, pixels, 4 * width * height);

    // for (int y = 0; y < height; y++)
    // for (int x = 0; x < width; x++)
    // {
    //     uint32_t pixel = pixels[x + y * width];
    //     int y2 = height - 1 - y;
    //     instance->_pixels[x + y2 * width] = pixel;
    // }
}
void BitmapWindow_SetPixelsScaled(BitmapWindow* instance, uint32_t* pixels, int width, int height, int scale)
{
    if (!BitmapWindow_Exists(instance)) return;

    for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
    {
        uint32_t pixel = pixels[x + y * width];
        // pixel = PixelToBwPixel(pixel);
        int x2 = x * scale;
        int y2 = y * scale;
        for (int i = 0; i < scale; i++)
        for (int j = 0; j < scale; j++)
            BitmapWindow_SetPixel(instance, x2+i, y2+j, pixel);
    }
}
void BitmapWindow_SetPixelsScaled2(BitmapWindow* instance, uint8_t* pixels, int width, int height, int scale)
{
    if (!BitmapWindow_Exists(instance)) return;

    for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
    {
        uint32_t pixel = pixels[x + y * width];

        float fraction = (float)pixel / 255;
        pixel = 0x00FFFFFF * fraction;

        int x2 = x * scale;
        int y2 = y * scale;
        for (int i = 0; i < scale; i++)
        for (int j = 0; j < scale; j++)
            BitmapWindow_SetPixel(instance, x2+i, y2+j, pixel);
    }
}
bool BitmapWindow_KeyDown_W(BitmapWindow* instance) { return instance->keydown_W; }
bool BitmapWindow_KeyDown_A(BitmapWindow* instance) { return instance->keydown_A; }
bool BitmapWindow_KeyDown_S(BitmapWindow* instance) { return instance->keydown_S; }
bool BitmapWindow_KeyDown_D(BitmapWindow* instance) { return instance->keydown_D; }
bool BitmapWindow_KeyDown_E(BitmapWindow* instance) { return instance->keydown_E; }
bool BitmapWindow_KeyDown_Q(BitmapWindow* instance) { return instance->keydown_Q; }
bool BitmapWindow_KeyDown_UP(BitmapWindow* instance) { return instance->keydown_UP; }
bool BitmapWindow_KeyDown_LEFT(BitmapWindow* instance) { return instance->keydown_LEFT; }
bool BitmapWindow_KeyDown_DOWN(BitmapWindow* instance) { return instance->keydown_DOWN; }
bool BitmapWindow_KeyDown_RIGHT(BitmapWindow* instance) { return instance->keydown_RIGHT; }
