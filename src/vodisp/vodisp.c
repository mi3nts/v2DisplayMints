#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <sys/shm.h>
#include <sys/ipc.h>

#include "vodisp.h"

static char *framebuffer;
volatile int auto_refresh;
pthread_t pid;

#define PAGEDSIZE   (((FRAMESIZE + 0x1000) / 0x1000) * 0x1000)

void vodisp_framebuffer_refresh_thread()
{
    while (auto_refresh) {
        if (framebuffer == NULL)
            return;
        vodisp_write_frame(framebuffer);
    }
    pthread_exit(0);
}

EXPORT_API char* vodisp_get_framebuffer(char *fb)
{
    if (framebuffer)
        return framebuffer;

    if (fb == NULL || *fb == 0) {
        framebuffer = malloc(PAGEDSIZE);
        memset(framebuffer, 0xff, FRAMESIZE);
    } else {
        // we have two method, normally shared memory speed is higher than mmap.
        // so if we need frame buffer, we can enable mmap, else use shm is better.
#ifdef USE_MMAP
        int fd = open(fb, O_CREAT | O_RDWR, 0666);
        if (fd <= 0)
            return NULL;
        ftruncate(fd, PAGEDSIZE);       // this is necessary.
        framebuffer = mmap(0, PAGEDSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (framebuffer == MAP_FAILED)
            framebuffer = NULL;
        close(fd);
#else
        // the path of fb must exists, can be empty file.
        close(open(fb, O_CREAT | O_EXCL, 0666));

        key_t key = ftok(fb, 0);
        int id = shmget(key, PAGEDSIZE, IPC_CREAT | 0666);
        if (id < 0)
            return NULL;
        framebuffer = shmat(id, NULL, 0);
        if (framebuffer == MAP_FAILED)
            framebuffer = NULL;
#endif
    }

    return framebuffer;
}

EXPORT_API int vodisp_get_auto_refresh()
{
    return auto_refresh;
}

EXPORT_API int vodisp_set_auto_refresh(int enable)
{
    // disable framebuffer update thread.
    if (!enable) {
        auto_refresh = 0;

        if (pid) {   // wait thread end.
            pthread_join(pid, NULL);
            pid = 0;
        }
    }

    // enable framebuffer update thread.
    else {
        int code;
        if (pid)        // already enabled.
            return -EEXIST;

        // enable auto refresh.
        auto_refresh = 1;

        code = pthread_create(&pid, NULL, (void *)vodisp_framebuffer_refresh_thread, NULL);
        if (code)
            return code < 0 ? code : -code;
    }

    return 1;
}

int vodisp_load_bitmap(const char *path, char *buf)
{
    FILE *fp = fopen(path, "rb");

    if (fp == NULL)
        return 0;

    // skip first 54 bit if it is bitmap file.
    if (strstr(path, ".bmp"))
        fseek(fp, 54, SEEK_SET);
    fread(buf, 1, FRAMESIZE, fp);
    fclose(fp);

    return FRAMESIZE;
}
