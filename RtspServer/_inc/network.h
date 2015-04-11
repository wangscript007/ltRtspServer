#ifndef NETWORK_H
#define NETWORK_H


#include <errno.h>

#include <signal.h>
#ifndef WIN32
#include <netinet/in.h>
# ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#include <sys/socket.h>
#endif

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include <pthread.h>
#include <queue>
#define MAX_WORK 4
//
enum TransType
{
    TCP,
    UDP,
    RTSP,
    RTP
};

typedef struct _WorkThread
{
    evutil_socket_t evpush[2];
    struct event_base	*base;
    struct event *evn;
    event_callback_fn callback;
    unsigned pid;
    pthread_t ptid;
}TWork;

class Network
{
public:
    explicit Network(struct sockaddr_in addr);
    bool NetPrepare(TransType type = TCP);
    bool StartServer(event_callback_fn readcb, event_callback_fn writecb);
    ~Network();
private:
    static void Signal_Cb(evutil_socket_t sig, short events, void *user_data);
    static void Listen_Cb(evconnlistener *listen, evutil_socket_t fd,
                          sockaddr *sa, int socklen, void *user_data);
    static void Tcp_Read_Cb(struct bufferevent* bev, void *arg);
    static void Tcp_Write_Cb(struct bufferevent* bev, void *arg);
    static void Event_Cb(struct bufferevent *bev, short events, void *user_data);
    static void* MultiWork(void* pvoid);
    struct evconnlistener* evconnlistener_new_udp_bind(evconnlistener_cb cb);

    struct event_base* evnbase;
    struct evconnlistener* listener;
    struct event* evn;

    event_callback_fn readcb,writecb;
    TWork twork[MAX_WORK];
    int sockfd;
    struct sockaddr_in sockinfo;
};



typedef struct _QNode
{
    TransType type;
    short events;
    void *forbev;
    void *arg;
}QNode,*PQNode;


class MesQueue
{
public:
    void MulticastInfo();

    void InsertQueue(PQNode InsNode);

    PQNode OutQueue();

    bool InsertWork(evutil_socket_t fd);

    bool DeleteWork(evutil_socket_t fd);

    static MesQueue* GetInstance();
protected:
    MesQueue();
    void MesLock();
    void MesUnLock();
    ~MesQueue();
protected:
    pthread_mutex_t QueueLock;
    evutil_socket_t workfd[MAX_WORK];
    std::queue<PQNode> QList;
    unsigned char WorkCount;
};

#endif // NETWORK_H
