#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <X11/Xlib.h>

void querypointer(Display *dpy, Window root, int* x, int* y);
void click(Display* dpy, Window root, int button);

int main(int __attribute__((unused)) argc, char* argv[])
{
    Display *dpy = XOpenDisplay(NULL);
    if(!dpy) {
        printf("Couldn't open display.\n"
               " - If you're running through SSH, try `DISPLAY=localhost:0.0 %s`.\n"
               " - If that didn't help, see http://unix.stackexchange.com/a/10126.\n", argv[0]);
        return 1;
    }

    Window root = DefaultRootWindow(dpy);

    // Not really needed, but I keep it here for quick reference.
    for(int screen = 0 ; screen < ScreenCount(dpy) ; ++screen) {
        int width = DisplayWidth(dpy, screen);
        int height = DisplayHeight(dpy, screen);
        printf("Screen %d: %dx%d\n", screen, width, height);
    }

    // Get the current cursor position.
    int x, y;
    querypointer(dpy, root, &x, &y);
    printf("Pre: %d,%d\n", x, y);

    // Jump the cursor to 10,10 for now.
    if(!isatty(STDIN_FILENO)) {
        if(2 != scanf("%d %d", &x, &y)) {
            printf("Error in your input!\n");
            return 1;
        }
    } else
        x = y = 10;
    XWarpPointer(dpy, None, root, 0, 0, 0, 0, x, y);
    XFlush(dpy);

    click(dpy, root, Button1);

    // Check if the jump did really happen?
    querypointer(dpy, root, &x, &y);
    printf("Post: %d,%d\n", x, y);

    XCloseDisplay(dpy);
    return 0;
}

// Return the current cursor x/y coordinates, relative to `w`.
void querypointer(Display *dpy, Window w, int* x, int* y)
{
    Window _root, _child;
    int _win_x, _win_y;
    unsigned int _mask;
    if(!XQueryPointer(dpy, w, &_root, &_child, x, y, &_win_x, &_win_y, &_mask)) {
        printf("Couldn't query pointer?\n");
    }
}

void click(Display *dpy, Window w, int button)
{
    XEvent event;
    memset(&event, 0, sizeof(event));
    event.type = ButtonPress;
    event.xbutton.button = button;
    event.xbutton.same_screen = True;

    // Find out the actual window (i.e. widget) the mouse is on. The event needs it!
    event.xbutton.subwindow = w;
    while(event.xbutton.subwindow) {
        event.xbutton.window = event.xbutton.subwindow;
        XQueryPointer(dpy, event.xbutton.window, &event.xbutton.root, &event.xbutton.subwindow, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
    }

    if(XSendEvent(dpy, PointerWindow, True, 0xfff, &event) == 0)
        printf("Couldn't send ButtonPress event?\n");
    XFlush(dpy);

    event.type = ButtonRelease;
    if(XSendEvent(dpy, PointerWindow, True, 0xfff, &event) == 0)
        printf("Couldn't send ButtonRelease event?\n");
    XFlush(dpy);
}
