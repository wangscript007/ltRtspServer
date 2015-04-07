#include "stdhead.h"
#include "network.h"
#include "MediaSession.h"

Network::Network(struct sockaddr_in addr)
{
    sockinfo = addr;
    evnbase = event_base_new();

    if(!evnbase)
    {
        //counld not init event base
    }
}

struct evconnlistener* Network::evconnlistener_new_udp_bind(evconnlistener_cb cb)
{
    struct evconnlistener *onlistener;
    evutil_socket_t fd;
    fd = socket(sockinfo.sin_family, SOCK_STREAM, 0);
    if(fd == -1)
        return NULL;
    if(evutil_make_socket_nonblocking(fd) < 0 || evutil_make_socket_closeonexec(fd) < 0
            || evutil_make_listen_socket_reuseable(fd) < 0){
        evutil_closesocket(fd);
        return NULL;
    }

    if(bind(fd, (struct sockaddr *)&sockinfo, sizeof(sockinfo)) < 0){
        perror("bad bind:");
        evutil_closesocket(fd);
        return NULL;
    }

    onlistener = evconnlistener_new(evnbase, cb, (void*)this,
                                    LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1, fd);
    if(!onlistener)
    {
        evutil_closesocket(fd);
        return NULL;
    }
    return onlistener;
}

bool Network::NetPrepare(TransType type)
{
    if(type == TCP || type == RTSP)
    {
        listener = evconnlistener_new_bind(evnbase, Listen_Cb,
                                           (void *)this, LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
                                           -1, (struct sockaddr*)&sockinfo, sizeof(sockinfo));
        if(!listener)
          {
            printf("error listen\n");
            return false;
          }
        return true;
    }
    else if(type == RTP || type == UDP)
    {
        listener = evconnlistener_new_udp_bind(Listen_Cb);
        if(!listener)
          {
            printf("udp error\n");
            return false;
          }
        return true;
    }
    return false;
}

bool Network::StartServer(bufferevent_data_cb preadcb, bufferevent_data_cb pwritecb)
{
    evn = evsignal_new(evnbase, SIGINT, Signal_Cb, (void *)evnbase);
    if(evn == NULL || event_add(evn, NULL) < 0){ return false; }
    readcb = preadcb;
    writecb = pwritecb;
    event_base_dispatch(evnbase);
	return true;
}

void Network::Signal_Cb(evutil_socket_t sig, short events, void *user_data)
{
  struct event_base *base = (struct event_base *)user_data;
  struct timeval delay = { 2, 0 };

  printf("Caught an interrupt signal; exiting cleanly in two seconds.\n");

  event_base_loopexit(base, &delay);
}

void Network::Listen_Cb(evconnlistener *listen, evutil_socket_t fd,
                               sockaddr *sa, int socklen, void *user_data)
{
    printf("a new connect \n");
    Network *net = (Network *)user_data;
    bufferevent *bev;

    bev = bufferevent_socket_new(net->evnbase, fd, BEV_OPT_CLOSE_ON_FREE);

    if(!bev){
        printf("error when create buff sock\n");return ;
    }

	MediaSession* session = new MediaSession(net);

    bufferevent_setcb(bev, net->readcb, net->writecb, NULL, session);
    bufferevent_enable(bev, EV_READ | EV_WRITE);

}


