#include "packetqueue.h"

PacketQueue::PacketQueue(): next(NULL), last(NULL), nb_packets(0), size(0), mutex(SDL_CreateMutex()), cond(SDL_CreateCond())
{

}

int PacketQueue::getNb_packets() const
{
    return nb_packets;
}

int PacketQueue::getSize() const
{
    return size;
}

int PacketQueue::packet_queue_put(PacketQueue *queue, AVPacket *packet)
{
    AVPacketList* pkt_list;
    if(av_dup_packet(packet) < 0){
        return -1;
    }
    pkt_list = (AVPacketList*)av_malloc(sizeof(AVPacketList));
    if(!pkt_list){
        return -1;
    }
    pkt_list->pkt = *packet;
    pkt_list->next = NULL;
    SDL_LockMutex(this->mutex);
    if(queue->last == NULL){
        queue->next = pkt_list;
    }else{
        queue->last->next = pkt_list;
    }
    queue->last = pkt_list;
    queue->nb_packets++;
    queue->size +=  packet->size;
    SDL_CondSignal(queue->cond);
    SDL_UnlockMutex(this->mutex);
    return 0;
}

int PacketQueue::packet_queue_get(PacketQueue *queue, AVPacket *pkt, int block)
{
    AVPacketList*  pkt_list = NULL;
    int ret = 0;
    SDL_LockMutex(mutex);
    while(true){
        pkt_list = queue->next;
        if(pkt_list != NULL){
            queue->next = queue->next->next;
            if(queue->next == NULL){
                queue->last = NULL;
            }
            queue->nb_packets--;
            queue->size -= pkt_list->pkt.size;
            *pkt = pkt_list->pkt;
            av_free(pkt_list);
            ret = 1;
            break;
        }
        else if(block == 0){
            ret = 0;
            break;
        }
        else{
            SDL_CondWait(cond, mutex);
        }
    }
    SDL_UnlockMutex(mutex);
    return ret;
}
