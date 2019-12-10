#ifndef LIBVODISP_H
#define LIBVODISP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FRAMESIZE   1152000     // 800 x 480 x 3

#define EXPORT_API __attribute__ ((visibility("default")))

EXPORT_API int vodisp_connect();
EXPORT_API int vodisp_disconnect();
EXPORT_API int vodisp_write_frame(const char *);

#ifdef __cplusplus
}
#endif

#endif // LIBVODISP_H
