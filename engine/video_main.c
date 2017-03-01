#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include "common.h"
#include "video_main.h"

Display *dpy;
int scrnum;
Window win;
GLXContext ctx;

/*
** Video_Init
*/
void Video_Init(void) {

    GL_SetMode();

};



/*
** GL_SetMode
*/
int GL_SetMode(void) {
    int width, height;

    int attrib[] = {
            GLX_RGBA,
            GLX_RED_SIZE, 1,
            GLX_GREEN_SIZE, 1,
            GLX_BLUE_SIZE, 1,
            None
    };

    Window root;
    XVisualInfo *visinfo;
    XSetWindowAttributes attr;
    unsigned long mask;

    Comm_Printf("Initializing OpenGL display\n");

    width = 600;
    height = 480;

    dpy = XOpenDisplay(NULL);
    scrnum = DefaultScreen(dpy);
    root = RootWindow(dpy, scrnum);
    visinfo = glXChooseVisual(dpy, scrnum, attrib);

    attr.background_pixel = 0;
    attr.border_pixel = 0;
    attr.colormap = XCreateColormap(dpy, root, visinfo->visual, AllocNone);
    attr.event_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | ButtonMotionMask | VisibilityChangeMask | StructureNotifyMask;

    mask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;

    win = XCreateWindow(dpy, root, 0, 0, width, height, 0, visinfo->depth, InputOutput, visinfo->visual, mask, &attr);

    XMapWindow(dpy, win);

    ctx = glXCreateContext(dpy, visinfo, NULL, True);

    glXMakeCurrent(dpy, win, ctx);

    return 0;
};


/*
** GL_Shutdown
*/
void GL_Shutdown(void){
    if(dpy) {
        if (ctx) glXDestroyContext(dpy, ctx);
        if (win) XDestroyWindow(dpy, win);
        XCloseDisplay(dpy);
    };
};


/*
** XLateKey
*/
int XLateKey(XKeyEvent *ev) {
    int key = 0;
    char buf[64];
    KeySym  keysym;

    XLookupString(ev, buf, sizeof(buf), &keysym, 0);
    switch(keysym) {

        case XK_Escape: printf("ESC pressed. Exiting...\n"); exit(0);break;
        case XK_Right: printf("Right arrow pressed\n"); break;
        case XK_Left: printf("Left arrow pressed\n"); break;
        case XK_Up: printf("Up arrow pressed\n"); break;
        case XK_Down: printf("Down arrow pressed\n"); break;


    };



};


/*
** HandleEvents
*/

void HandleEvents(void) {

    XEvent event;

    while(XPending(dpy)) {
        XNextEvent(dpy, &event);

        switch(event.type) {
        case KeyPress:
            XLateKey(&event.xkey);
            printf("Key pressed\n");
            break;
        case KeyRelease:
            printf("Key released\n");
            break;
        case MotionNotify:
            printf("MotionNotify\n");
            break;
        case ButtonPress:
            printf("Button pressed\n");
            break;
        case ButtonRelease:
            printf("Button released\n");
            break;
        case CreateNotify:
            printf("CreateNotify\n");
            break;
        case ConfigureNotify:
            printf("ConfigureNotify\n");
            break;
        };
    };




};


