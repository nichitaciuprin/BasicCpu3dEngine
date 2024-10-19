#include "Std.h"
#include "StdExt.h"
#include "SysHelper.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

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

    // HWND       _hwnd;
    // HDC        _hdc;
    // HBITMAP    _hbitmap;
    int        _width;
    int        _height;
}
BitmapWindow;

bool windowClosed = false;
Atom wm_delete_window;
Display* display;
Window window;

BitmapWindow* BitmapWindow_Create(int x, int y, int clientWidth, int clientHeight)
{
    BitmapWindow* instance = (BitmapWindow*)malloc(sizeof(BitmapWindow));

    instance->keydown_W = 0;
    instance->keydown_A = 0;
    instance->keydown_S = 0;
    instance->keydown_D = 0;
    instance->keydown_E = 0;
    instance->keydown_Q = 0;
    instance->keydown_UP = 0;
    instance->keydown_LEFT = 0;
    instance->keydown_DOWN = 0;
    instance->keydown_RIGHT = 0;

    int width = clientWidth;
    int height = clientHeight;

    instance->_width = width;
    instance->_height = height;

    display = XOpenDisplay(NULL);
    if (display == NULL)
        abort();

    int screen = DefaultScreen(display);

    Window root = RootWindow(display, screen);

    int borderWidth = 1;

    unsigned long blackPixel = BlackPixel(display, screen);
    unsigned long whitePixel = WhitePixel(display, screen);

    window = XCreateSimpleWindow(display, root, 100, 100, width, height, borderWidth, blackPixel, whitePixel);

    // TODO
    // XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    // XEvent event;
    // XNextEEvent(display, &event);

    wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, window, &wm_delete_window, 1);

    return instance;
}
bool BitmapWindow_Exists(BitmapWindow* instance)
{
    return windowClosed;
}
void BitmapWindow_Destroy(BitmapWindow* instance)
{
    windowClosed = true;
    XDestroyWindow(display, window);
}
void BitmapWindow_Update(BitmapWindow* instance)
{
    XEvent event;

    int messageCount = XPending(display);

    for (size_t i = 0; i < messageCount; i++)
    {
        switch (event.type)
        {
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

                // XPutImage(display, window, gc, image, 0, 0, 0, 0, width, height);

                // XEvent se;
                // se.type = Expose;
                // se.xexpose.type = Expose;
                // se.xexpose.serial = 0;
                // se.xexpose.send_event = 1;
                // se.xexpose.window = window;
                // se.xexpose.x = 0;
                // se.xexpose.y = 0;
                // se.xexpose.width = width;
                // se.xexpose.height = height;
                // se.xexpose.count = 0;
                // XSendEvent(display, window, false, 0, &se);
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

            case ClientMessage:
            {
                cout << "!" << endl;

                if (event.xclient.data.l[0] == wm_delete_window)
                {
                    // on_delete(event.xclient.display, event.xclient.window);
                    // windowClosed = true;
                    // XDestroyWindow(display, window);
                }
            }
            break;

            default:
                printf("missed event %i\n", event.type);
                break;
        }
    }
}
