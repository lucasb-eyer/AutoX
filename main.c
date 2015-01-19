#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>

void querypointer(Display *dpy, Window root, int* x, int* y);

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
    int screen = DefaultScreen(dpy);
    int width = DisplayWidth (dpy, screen);
    int height = DisplayHeight(dpy, screen);
    printf("Screen wxh: %d %dx%d\n", screen, width, height);

    // Get the current cursor position.
    int x, y;
    querypointer(dpy, root, &x, &y);
    printf("Pre: %d,%d\n", x, y);

    // Jump the cursor to 10,10 for now.
    XWarpPointer(dpy, None, root, 0, 0, 0, 0, 10, 10);
    XFlush(dpy);

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
        puts("Couldn't query pointer?");
    }
}
