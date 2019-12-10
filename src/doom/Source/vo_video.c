//
// DESCRIPTION:
//	DOOM graphics stuff for VoCore Display, OpenWrt.
//  based on i_video.c.
//
//-----------------------------------------------------------------------------

#include "includes.h"
#include "openwrt/libvodisp.h"

unsigned int SCREENWIDTH = 320;
unsigned int SCREENHEIGHT = 240;
unsigned int ORIGSCREENWIDTH = 320;
unsigned int ORIGSCREENHEIGHT = 200;

union pixel {
    struct _rgb {
        unsigned char red;
        unsigned char green;
        unsigned char blue;
    }rgb;
    unsigned int full:24;
};
static union pixel palette_table[256];
static byte *framebuffer;

void I_InitGraphics (void)
{
    framebuffer = malloc(800 * 480 * 3);
    memset(framebuffer, 0, 800 * 480 * 3);

    vodisp_connect();
}

void I_ShutdownGraphics(void)
{
    vodisp_disconnect();
}

void I_SetPalette (byte* palette)
{
    unsigned int colour = 0;
    do {
        palette_table[colour].rgb.blue   = gammatable[usegamma][*palette++];
        palette_table[colour].rgb.green = gammatable[usegamma][*palette++];
        palette_table[colour].rgb.red  = gammatable[usegamma][*palette++];
    } while (++colour < 256);
}

void show_fps()
{
    static time_t last;
    static int fps;

    time_t now = time(NULL);
    if (last == now)
        fps++;
    else {
        last = now;
        printf("fps: %d\n", fps);    
        fps = 0;
    }
}

void I_FinishUpdate (void)
{
    byte *src = screens[0], *dst = framebuffer;
    int i, j, pos;

#ifndef SMALL_SCREEN
    int w, h;
    h = SCREENHEIGHT * 6;
    w = SCREENWIDTH * 6;

    for (j = 0; j < h; j += 6) {
        for (i = 0; i < w; i += 6) {
            pos = 800 * 480 * 3 - i * 480 + j;
            *(unsigned int *)(dst + pos    ) = palette_table[*src].full;
            *(unsigned int *)(dst + pos + 3) = palette_table[*src].full;
            *(unsigned int *)(dst + pos - 1440) = palette_table[*src].full;
            *(unsigned int *)(dst + pos - 1437) = palette_table[*src].full;
            src++;
        }
    }
#else
    for (j = 0; j < SCREENHEIGHT; j++) {
        pos = j * 480 * 3;
        for (i = 0; i < SCREENWIDTH; i++)
            *(unsigned int *)(dst + pos + i * 3) = palette_table[*src++].full;
    }
#endif

    vodisp_write_frame(framebuffer);
    //show_fps();
}

void I_ReadScreen (byte* scr)
{
    memcpy(scr, screens[0], SCREENWIDTH * SCREENHEIGHT);
}

void I_InitKeyboard (void)
{
}

void I_UpdateNoBlit (void)
{
}

void I_SetScreenSize (void)
{
}

void I_SetWindowName (const char * title)
{
}

void I_StartFrame (void)
{
}

void I_StartTic (void)
{
}

