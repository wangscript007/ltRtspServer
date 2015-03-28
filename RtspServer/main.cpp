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
    sa.sin_family = AF_INET;
    sa.sin_port = htons(554);
    const char* ver = event_get_version();
    printf("%s\n", ver);
    Network* work = new Network(sa);
    work->NetPrepare(TCP);
    work->StartServer(Read_cb, Write_cb);

    return 0;
}

