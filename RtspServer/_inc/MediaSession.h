#pragma once
#include "network.h"
#include "RtspString.h"
/************************************************************************/
/*               ý��Ự                                               */
/*	
/************************************************************************/
typedef long long INT64;
class MediaSession
{
public:
	
    static void Send(evutil_socket_t fd, short events, void *arg);
    static void Recv(evutil_socket_t fd, short events, void *arg);

    void DealRtsp(struct bufferevent* bev);
	void SetDescribe();
    static long long GenSessionID();
	explicit MediaSession(Network *pwork):work(pwork){};
protected:
	virtual ~MediaSession();
	rtsp_string rs;
private:
	Network *work;
    INT64 sessionid;
	std::string rtspinc;

	
};
