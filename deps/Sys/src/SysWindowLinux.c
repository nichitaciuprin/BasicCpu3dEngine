#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include <X11/Xlib.h>

typedef struct BitmapWindow
{
    int width;
    int height;

    bool windowClosed;
    Display* display;
    Window window;
    Atom wm_delete_window;
    uint32_t* pixels;
    XImage* image;

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
}
BitmapWindow;

BitmapWindow* BitmapWindow_Create(int x, int y, int clientWidth, int clientHeight)
{
    BitmapWindow* instance = (BitmapWindow*)malloc(sizeof(BitmapWindow));

    instance->windowClosed = false;

    instance->width = clientWidth;
    instance->height = clientHeight;

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
    XAutoRepeatOn(instance->display);
    int screen = DefaultScreen(instance->display);
    Window root = DefaultRootWindow(instance->display);
    instance->window = XCreateSimpleWindow(instance->display, root, 0, 0, instance->width, instance->height, 0, 0, 0xffffffff);
    XSelectInput(instance->display, instance->window, ExposureMask | KeyPressMask | KeyReleaseMask);
    // XSelectInput(instance->display, instance->window, ExposureMask | ButtonPressMask);
    XMapWindow(instance->display, instance->window);
    instance->wm_delete_window = XInternAtom(instance->display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(instance->display, instance->window, &instance->wm_delete_window, 1);

    Visual* visual = DefaultVisual(instance->display, screen);
    int depth = DefaultDepth(instance->display, screen);

    instance->pixels = (uint32_t*)malloc(4 * instance->width * instance->height);

    instance->image = XCreateImage
    (
        instance->display, visual, depth, ZPixmap, 0,
        (char*)instance->pixels, instance->width, instance->height, 32, 0
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

    while (XPending(instance->display))
    {
        XNextEvent(instance->display, &event);

        switch (event.type)
        {
            case ClientMessage:
            {
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
                if (instance->windowClosed) break;

                // TODO should be cached?
                int screen = DefaultScreen(instance->display);
                GC defaultGc = DefaultGC(instance->display, screen);

                XPutImage(instance->display, instance->window, defaultGc, instance->image, 0, 0, 0, 0, instance->width, instance->height);

                XEvent se;
                se.type = Expose;
                se.xexpose.type = Expose;
                se.xexpose.serial = 0;
                se.xexpose.send_event = 1;
                se.xexpose.window = instance->window;
                se.xexpose.x = 0;
                se.xexpose.y = 0;
                se.xexpose.width = instance->width;
                se.xexpose.height = instance->height;
                se.xexpose.count = 0;

                XSendEvent(instance->display, instance->window, false, 0, &se);
            }
            break;

            case KeyPress:
            case KeyRelease:
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

                bool value = event.type == KeyPress;

                switch (keyKode)
                {
                    case 0x19: { instance->keydown_W     = value; } break;
                    case 0x26: { instance->keydown_A     = value; } break;
                    case 0x27: { instance->keydown_S     = value; } break;
                    case 0x28: { instance->keydown_D     = value; } break;
                    case 0x6f: { instance->keydown_UP    = value; } break;
                    case 0x71: { instance->keydown_LEFT  = value; } break;
                    case 0x74: { instance->keydown_DOWN  = value; } break;
                    case 0x72: { instance->keydown_RIGHT = value; } break;
                    case 0x18: { instance->keydown_Q     = value; } break;
                    case 0x1a: { instance->keydown_E     = value; } break;
                }

                if (keyKode == esc)
                {
                    instance->windowClosed = true;
                    XDestroyWindow(instance->display, instance->window);
                }
            }
            break;

            default:
            {
                printf("missed event %i\n", event.type);
            }
            break;
        }
    }
}
void BitmapWindow_SetPixel(BitmapWindow* instance, int x, int y, uint32_t pixel)
{
    if (!BitmapWindow_Exists(instance)) return;

    instance->pixels[x + y * instance->width] = pixel;
}
void BitmapWindow_SetPixels(BitmapWindow* instance, uint32_t* pixels, int width, int height)
{
    if (!BitmapWindow_Exists(instance)) return;

    memcpy(instance->pixels, pixels, 4 * width * height);

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
