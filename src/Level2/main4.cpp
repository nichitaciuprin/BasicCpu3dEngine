#include "Std.h"
#include "StdExt.h"
#include "SysHelper.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

int main()
{
    int width = 512;
    int height = 512;

    uint32_t pixels[width * height] = {};

    Display* display = XOpenDisplay(NULL);
    if (display == NULL)
        abort();

    int screen = DefaultScreen(display);

    Window root = RootWindow(display, screen);

    int borderWidth = 1;

    unsigned long blackPixel = BlackPixel(display, screen);
    unsigned long whitePixel = WhitePixel(display, screen);

    XSetWindowAttributes attributes = {};
    attributes.border_pixel = 0;

    Window window = XCreateSimpleWindow(display, root, 100, 100, width, height, borderWidth, blackPixel, whitePixel);

    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, window, & wm_delete_window, 1);

    Visual* visual = DefaultVisual(display, screen);
    int depth = DefaultDepth(display, screen);
    GC gc = DefaultGC(display, screen);

    XImage* image = XCreateImage
    (
        display, visual, depth, ZPixmap, 0,
        (char*)pixels, width, height, 32, 0
    );

    if (image == NULL)
        abort();

    int quit = 0;

    while (!quit)
    {
        XEvent event;
        XNextEvent(display, &event);

        switch (event.type)
        {
            case Expose:
            {
                long time1 = GetTime();

                for (int x = 0; x < width; x++)
                for (int y = 0; y < height; y++)
                {
                    uint8_t r = (time1 / 10) % 255;
                    uint8_t g = (time1 / 10) % 255;
                    uint8_t b = 0;
                    uint8_t a = 0;

                    uint32_t pixel = 0;

                    pixel += a; pixel <<= 8;
                    pixel += r; pixel <<= 8;
                    pixel += g; pixel <<= 8;
                    pixel += b;

                    pixels[y * width + x] = pixel;
                }

                XPutImage(display, window, gc, image, 0, 0, 0, 0, width, height);

                XEvent se;
                se.type = Expose;
                se.xexpose.type = Expose;
                se.xexpose.serial = 0;
                se.xexpose.send_event = 1;
                se.xexpose.window = window;
                se.xexpose.x = 0;
                se.xexpose.y = 0;
                se.xexpose.width = width;
                se.xexpose.height = height;
                se.xexpose.count = 0;
                XSendEvent(display, window, false, 0, &se);
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

                if (keyKode == esc)
                    quit = 1;
            }
            break;

            default:
                printf("missed event %i\n", event.type);
                break;
        }
    }

    if (image)
    {
        // XDestroyImage tries to free the image data, even though create doesn't allocate it
        // Set the data to NULL to avoid this
        image->data = NULL;
        XDestroyImage(image);
    }

    if (display)
    {
        XDestroyWindow(display, window);
        XCloseDisplay(display);
    }

    return 0;
}
