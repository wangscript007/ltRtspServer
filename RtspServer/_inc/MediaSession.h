#pragma once
#include "network.h"
#include "RtspString.h"
/************************************************************************/
/*               Γ½Με»α»°                                               */
/*	
/************************************************************************/

class MediaSession
{
public:
	
	static void Send(struct bufferevent *bev, void *ctx);
	static void Recv(struct bufferevent *bev, void *ctx);

	explicit MediaSession(Network *pwork):work(pwork){};
protected:
	virtual ~MediaSession();
	rtsp_string rs;
private:
	Network *work;
	std::string rtspinc;
	
};