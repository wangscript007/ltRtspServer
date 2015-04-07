#include <iostream>
#include "_inc/stdhead.h"
#include "_inc/network.h"
#include "_inc/RtspString.h"
#include "_inc/MediaSession.h"
#include <smart_ptr/boost/shared_ptr.hpp>
using namespace std;

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
    work->StartServer(MediaSession::Recv, MediaSession::Send);

    return 0;
}

