#include "../_inc/stdhead.h"
#include "../_inc/network.h"


Network::Network(struct sockaddr_in addr)
{
    assert(addr != NULL);
    sockinfo = addr;
    evnbase = event_base_new();
    if(!base)
    {
        //counld not init event base
    }
}

struct evconnlistener* Network::evconnlistener_new_udp_bind(evconnlistener_cb cb)
{
    struct evconnlistener *onlistener;
    evutil_socket_t fd;
    fd = socket(sockinfo.sin_family, SOCK_DGRAM, 0);
    if(fd == -1)
        return NULL;
    if(evutil_make_socket_nonblocking(fd) < 0 || evutil_make_socket_closeonexec(fd) < 0
            || evutil_make_listen_socket_reuseable(fd) < 0){
        evutil_closesocket(fd);
        return NULL;
    }

    if(bind(fd, (struct sockaddr)sockinfo, sizeof(sockinfo)) < 0){
        evutil_closesocket(fd);
        return NULL;
    }
    onlistener = evconnlistener_new(evnbase, cb, (void*)base, LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1, fd);
    if(!onlistener)
    {
        evutil_closesocket(fd);
        return NULL;
    }
    return onlistener;
}

bool Network::NetPrepare(TransType type, unsigned needlisten, evconnlistener_cb pfunc)
{
    if(type == TCP || type == RTSP)
    {

        listener = evconnlistener_new_bind(evnbase, pfunc,
                                           (void *)base, LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
                                           -1, (struct sockaddr*)&sockinfo, sizeof(sockinfo));
        if(!listener)
            return false;
        return true;
    }
    else if(type == RTP || type == UDP)
    {
        listener = evconnlistener_new_udp_bind(pfunc);
        if(!listener)
            return false;
        return true;
    }
    return false;
}
