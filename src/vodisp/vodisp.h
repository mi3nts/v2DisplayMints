#ifndef VODISP_H
#define VODISP_H

#include <stdint.h>
#include "libvodisp.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FRAMESIZE   1152000     // 800 x 480 x 3

#define EXPORT_API __attribute__ ((visibility("default")))

EXPORT_API char* vodisp_get_framebuffer(char *);
EXPORT_API int vodisp_set_auto_refresh(int);
EXPORT_API int vodisp_get_auto_refresh();
EXPORT_API int vodisp_load_bitmap(const char *path, char *buf);

#ifdef __cplusplus
}
#endif

#endif // VODISP_H
