#ifndef NETWORK_H
#define NETWORK_H
#include <sys/socket.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/util.h>
#include <event2/listener.h>
#include <signal.h>
//
enum TransType
{
    TCP,
    UDP,
    RTSP,
    RTP
};

class Network
{
public:
    explicit Network(struct sockaddr_in addr);
    bool NetPrepare(TransType type = TCP, unsigned needlisten = 1, void* pfunc = NULL);
private:
    struct evconnlistener* evconnlistener_new_udp_bind(evconnlistener_cb cb);

    struct event_base* evnbase;
    struct evconnlistener* listener;
    struct event* evn;
    int sockfd;
    struct sockaddr_in sockinfo;
};

#endif // NETWORK_H
