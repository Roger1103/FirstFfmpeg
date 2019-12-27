//
// Created by iotimc on 2019/12/21.
//

#ifndef FIRSTFFMPEG_DEMUX_H
#define FIRSTFFMPEG_DEMUX_H

#endif //FIRSTFFMPEG_DEMUX_H

class Demux {
protected:
    struct AVFormatContext *ic;
    int videoStream = 0;
    int audioStream = 1;
public:
    virtual void init(){};

    virtual void open(const char *url){};

    virtual bool read(){};

    virtual void getVideoParams(){};

    virtual void getAudioParams(){};
};