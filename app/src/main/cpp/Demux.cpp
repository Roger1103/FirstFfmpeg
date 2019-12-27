//
// Created by iotimc on 2019/12/21.
//

#include "Demux.h"
#include <libavformat/avformat.h>
#include <android/log.h>

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
    pkt = av_packet_alloc();
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

void Demux::codec(){
    //临时存储的解码器上下文
    AVCodecContext *cc = NULL;
    //视频解码器
    AVCodecContext *vc = NULL;
    //视频解码器
    AVCodecContext *ac = NULL;
    int ret;

    avcodec_register_all();

    //得到视音频解码器
    AVCodec *audioCodec = avcodec_find_decoder(
            ic->streams[audioStream]->codecpar->codec_id);
    //得到视频解码器
    AVCodec *videoCodec = avcodec_find_decoder(
            ic->streams[videoStream]->codecpar->codec_id);

    ret = avcodec_parameters_to_context(ac, ic->streams[audioStream]->codecpar);
    if (ret < 0) {
        return;
    }

    ret = avcodec_parameters_to_context(vc,ic->streams[videoStream]->codecpar);
    if (ret < 0) {
        return;
    }

    //打开解码器
    ret = avcodec_open2(ac, audioCodec, 0);
    if (ret != 0) {
        return;
    }


    ret = avcodec_open2(vc, videoCodec, 0);
    if (ret != 0) {
        return;
    }
    AVFrame *avFrame = av_frame_alloc();

    //开始解码
    for(;;){
        //得到解封装后的 AvPacket
        ret = av_read_frame(ic, pkt);
        if(ret!=0){
            continue;
        }

        if(pkt->stream_index == audioStream){//当前解码音频数据
            cc = ac;
        }else if(pkt->stream_index == videoStream){//当前解码视频帧
            cc = vc;
        }

        //将 AvPacket 送入给解码队列
        ret = avcodec_send_packet(cc, pkt);


        //得到解码后的 AvFrame 数据
        //发送一个 avpacket 之后可能可以收到多个 avframe
        for(;;){
            ret = avcodec_receive_frame(cc, avFrame);
            if (ret != 0) {
                break;
            }
            //TODO 在这里可以处理解码后的数据拉，例如滤镜操作，像素格式转化，重采样等。
        }
    }

     //释放资源
    avcodec_free_context(&ac);
    avcodec_free_context(&vc);

}


