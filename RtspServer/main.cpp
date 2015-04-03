#include <iostream>
#include "_inc/stdhead.h"
#include "_inc/network.h"
#include "_inc/RtspString.h"
using namespace std;

static char stringget[1024] = {0};
static std::string tc = "";
static void Read_cb(struct bufferevent *bev, void *ctx)
{
  //printf("get a mes\n");
  memset(stringget, 0, 1024);
  bufferevent_read(bev, stringget, 1024);
  printf("st get :\n%s\n", stringget);
  tc.append(stringget);

  if(tc.find("\r\n\r\n") != std::string::npos )
  {
      rtsp_string rs;
      rs.deal_requset(tc);
      printf("st deal :\n%s\n", tc.c_str());
      bufferevent_write(bev, tc.c_str(), tc.size());
      bufferevent_write(bev, "\r\n", 2);
  }
}


static void Write_cb(struct bufferevent *bev, void *ctx)
{
  //printf("get a mes\n");
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

