/* Generated by GRAHAM */
#ifndef AVUTIL_AVCONFIG_H
#define AVUTIL_AVCONFIG_H
#define AV_HAVE_BIGENDIAN 0
#define AV_HAVE_FAST_UNALIGNED 0

#if defined(TARGET_OSX)
#define HAVE_CBRTF	1
#define HAVE_ISINF	1
#define HAVE_ISNAN	1
#define HAVE_RINT	1
#define HAVE_LRINT	1
#define HAVE_LRINTF	1
#define HAVE_ROUND	1
#define HAVE_ROUNDF	1
#define HAVE_TRUNC	1
#define HAVE_TRUNCF	1
#endif

#include <libavutil/internal.h>
#include <libavformat/avio.h>

#endif /* AVUTIL_AVCONFIG_H */