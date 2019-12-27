#include <jni.h>
#include <string>
#include <zconf.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}
#include "Demux.h"

static Demux *demux;

extern "C" JNIEXPORT jstring JNICALL
Java_com_aiot_device_firstffmpeg_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = avcodec_configuration();
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_aiot_device_firstffmpeg_FFmpegUtil_init(JNIEnv *env, jclass type) {
    // TODO
    if(!demux){
        demux=new Demux();
        demux->init();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_aiot_device_firstffmpeg_FFmpegUtil_open(JNIEnv *env, jclass type, jstring url_) {
    const char *url = env->GetStringUTFChars(url_, 0);
    // TODO
    if(!demux){
        demux->open(url);
    }
    env->ReleaseStringUTFChars(url_, url);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_aiot_device_firstffmpeg_FFmpegUtil_read(JNIEnv *env, jclass type) {
    // TODO
    if (!demux) {
        return;
    }
    bool re = true;
    while (re) {
        re = demux->read();
    }
}





