#include <iostream>
#include "_inc/stdhead.h"
#include "_inc/network.h"
using namespace std;



static void Read_cb(struct bufferevent *bev, void *ctx)
{
  printf("get a mes\n");
}


static void Write_cb(struct bufferevent *bev, void *ctx)
{
  printf("get a mes\n");
}



int main()
{
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(1000);
    sa.sin_addr.s_addr = htonl(0);
    const char* ver = event_get_version();
    printf("%s\n", ver);
    Network* work = new Network(sa);
    work->NetPrepare(TCP);
    work->StartServer(Read_cb, Write_cb);

    return 0;
}

