#include "stdafx.h"
#include "MediaSession.h"


void MediaSession::Recv(struct bufferevent *bev, void *ctx)
{
	MediaSession *session = (MediaSession *)ctx;
	char pstring[128] = {0};
	
	if(bufferevent_read(bev, pstring, 128))
	{
		session->rtspinc.append(pstring);
		if ( session->rtspinc.find("\r\n\r\n") != std::string::npos )
		{
			session->rs.deal_requset(session->rtspinc);
			bufferevent_write( bev, session->rtspinc.c_str(), session->rtspinc.size() );
			bufferevent_write( bev, "\r\n", 2);
		}
	}
}

void MediaSession::Send(struct bufferevent *bev, void *ctx)
{
	MediaSession *session = (MediaSession *)ctx;

	//session->rtspinc.clear();
}



MediaSession::~MediaSession()
{

}

