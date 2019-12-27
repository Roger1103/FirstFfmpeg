//
// Created by iotimc on 2019/12/21.
//

#include "Demux.h"
#include <libavformat/avformat.h>


void Demux::init() {
    //注册所有封装器
    av_register_all();
    //初始化网络
    avformat_network_init();
}

void Demux::open(const char *url) {
    //打开文件
    int re = avformat_open_input(&ic, url, 0, 0);
    if (re != 0) {
        char buff[1024] = {0};
        av_strerror(re, buff, sizeof(buff));
        return;
    }


    //读取文件信息
    re = avformat_find_stream_info(ic, 0);
    if (re != 0) {
        char buff[1024] = {0};
        av_strerror(re, buff, sizeof(buff));
        return;
    }
    //读取总时长
    int64_t totalMs = ic->duration / (AV_TIME_BASE / 1000);

    getVideoParams();
    getAudioParams();
}

void Demux::getVideoParams() {
    if (!ic) {
        return;
    }
    int re = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, 0, 0);
    if (re < 0) {

        return;
    }
    videoStream = re;
}

void  Demux::getAudioParams() {
    if (!ic) {
        return;
    }
    int re = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, 0, 0);
    if (re < 0) {

        return;
    }
    audioStream = re;
}


//分数转为浮点数
static double r2d(AVRational r) {
    return r.num == 0 || r.den == 0 ? 0 : (double) r.num / (double) r.den;
}

bool Demux::read() {
    if (!ic) {
        return false;
    }
    AVPacket *pkt = av_packet_alloc();
    int re = av_read_frame(ic, pkt);
    if (re != 0) {
        av_packet_free(&pkt);
        return false;
    }
    pkt->pts = (long long) (pkt->pts * (1000 * r2d(ic->streams[pkt->stream_index]->time_base)));
    if (pkt->stream_index == audioStream) {

    } else if (pkt->stream_index == videoStream) {

    } else {
        av_packet_free(&pkt);
        return false;
    }
    av_packet_free(&pkt);
    return true;
}


