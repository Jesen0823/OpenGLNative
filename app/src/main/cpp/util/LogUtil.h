//
// Created by xie_s on 2024/5/24.
//

#ifndef OPENGLNATIVE_LOGUTIL_H
#define OPENGLNATIVE_LOGUTIL_H

#include<android/log.h>
#include <sys/time.h>

#define  LOG_TAG "ByteFlow"

#define  LOGCATE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGCATV(...)  __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG,__VA_ARGS__)
#define  LOGCATD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGCATI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

#define FUN_BEGIN_TIME(FUN) {\
    LOGCATE("%s:%s start", __FILE__, FUN); \
    long long t0 = GetSysCurrentTime();

#define FUN_END_TIME(FUN) \
    long long t1 = GetSysCurrentTime(); \
    LOGCATE("%s:%s cost time %ldms", __FILE__, FUN, (long)(t1-t0));}

static long long GetSysCurrentTime()
{
    struct timeval time;
    gettimeofday(&time, NULL);
    long long curTime = ((long long)(time.tv_sec))*1000+time.tv_usec/1000;
    return curTime;
}

#define GO_CHECK_GL_ERROR(...)   LOGCATE("CHECK_GL_ERROR %s glGetError = %ld, line = %d, ",  __FUNCTION__, glGetError(), __LINE__)

#endif //OPENGLNATIVE_LOGUTIL_H
