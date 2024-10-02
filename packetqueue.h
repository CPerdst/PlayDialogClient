#ifndef PACKETQUEUE_H
#define PACKETQUEUE_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/pixfmt.h"
#include "libswscale/swscale.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libavutil/time.h"
#include "SDL.h"
#ifdef __cplusplus
}
#endif

class PacketQueue
{
public:
    PacketQueue();
    int getNb_packets() const;
    int getSize() const;
    int packet_queue_put(PacketQueue* queue, AVPacket* packet);
    int packet_queue_get(PacketQueue *queue, AVPacket *pkt, int block);


private:
    AVPacketList* next;
    AVPacketList* last;
    int nb_packets;
    int size;
    SDL_mutex* mutex;
    SDL_cond* cond;
};

#endif // PACKETQUEUE_H
