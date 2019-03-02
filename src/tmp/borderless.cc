#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<X11/X.h>
#include<X11/Xlib.h>
#include<GL/gl.h>
#include<GL/glx.h>
#include<GL/glu.h>
 
Display                 *dpy;
Window                  root, win;
GLint                   att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
Colormap                cmap;
XVisualInfo             *vi;
GLXContext              glc;
 
int main(int argc, char *argv[]) {
 XEvent                 xev;
 XWindowAttributes      gwa;
 XSetWindowAttributes   swa;
 int                    cw_mask = CWEventMask | CWColormap | CWOverrideRedirect;
 
 dpy    = XOpenDisplay(NULL);
 root   = DefaultRootWindow(dpy);
 vi     = glXChooseVisual(dpy, 0, att);
 cmap   = XCreateColormap(dpy, root, vi->visual, AllocNone);
 glc    = glXCreateContext(dpy, vi, NULL, GL_TRUE);
 
 swa.override_redirect = True;
 swa.event_mask = ExposureMask;
 swa.colormap = cmap;
 
 win = XCreateWindow(dpy, root, 200, 200, 400, 400, 0, vi->depth, InputOutput, vi->visual, cw_mask, &swa);
 
 XMapWindow(dpy, win);
 glXMakeCurrent(dpy, win, glc);
 
 while(1) {
        XNextEvent(dpy, &xev);
 
        if(xev.type == Expose) {
                XGetWindowAttributes(dpy, win, &gwa);
                glViewport(0, 0, gwa.width, gwa.height);
                glClearColor(0.3, 1.0, 0.3, 1.0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
                glXSwapBuffers(dpy, win); } } }
//
//      gcc -o borderless borderless.cc -lX11 -lGL -lGLU
//
