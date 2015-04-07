#pragma once
#include "network.h"
#include "RtspString.h"
/************************************************************************/
/*               Γ½Με»α»°                                               */
/*	
/************************************************************************/
typedef long long INT64;
class MediaSession
{
public:
	
	static void Send(struct bufferevent *bev, void *ctx);
	static void Recv(struct bufferevent *bev, void *ctx);

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
