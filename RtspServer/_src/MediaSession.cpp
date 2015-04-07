#include "stdhead.h"
#include "MediaSession.h"


long long
MediaSession::GenSessionID()
{

	static LARGE_INTEGER	tickFrequency;
	static BOOL				tickFrequencySet = FALSE;

	LARGE_INTEGER	tickNow;

	if (tickFrequencySet == FALSE) 
	{
		QueryPerformanceFrequency(&tickFrequency);
		tickFrequencySet = TRUE;
	}
	QueryPerformanceCounter(&tickNow);

	//return (INT32)(tickNow.QuadPart / tickFrequency.QuadPart);
	return tickNow.QuadPart;
}

void 
MediaSession::SetDescribe()
{
	unsigned p = 0;
	char SessionId[48] = {0};
	sessionid = GenSessionID();
    snprintf(SessionId, 48, "%I64u", sessionid);
	//Ìæ»»
	while ((p = rtspinc.find("sessionforreplace")) != std::string::npos)
	{
		rtspinc.replace(p, 17, SessionId);
	}

	SessionId[0] = '9';
	SessionId[1] = '6';
	SessionId[2] = '\0';

	while ((p = rtspinc.find("loadtype")) != std::string::npos)
	{
		rtspinc.replace(p, 8, SessionId);
	}

	SessionId[0] = '0';
	SessionId[1] = '\0';
	rtspinc.replace(rtspinc.find("bandwidth"), 9, SessionId);

	SessionId[0] = '2';
	SessionId[1] = '0';
	SessionId[2] = '1';
	SessionId[3] = '\0';
	rtspinc.replace(rtspinc.find("tmpforesid"), 10, SessionId);

	SessionId[0] = '1';
	SessionId[1] = '\0';
	rtspinc.replace(rtspinc.find("tmpforstid"), 10, SessionId);

	p = rtspinc.find("\r\n\r\n");
	int ContentSize = rtspinc.size() - p;
    snprintf(SessionId, 48, "%d", ContentSize - 4);
	
	p = rtspinc.find("content_count");
	rtspinc.replace(p, 13, SessionId);

}


void 
MediaSession::Recv(struct bufferevent *bev, void *ctx)
{
	MediaSession *session = (MediaSession *)ctx;
	char pstring[1480] = {0};
	
	if(bufferevent_read(bev, pstring, 1480))
	{
		//printf("Get New Node:\n%s\n", pstring);
		session->rtspinc.append(pstring);

		if ( session->rtspinc.find("\r\n\r\n") != std::string::npos )
		{
			printf("IN<<\n%s\n", session->rtspinc.c_str());
			if(session->rs.deal_requset(session->rtspinc).type == DESCRIBE)
			{
				session->SetDescribe();
				int p = session->rtspinc.find("\r\n\r\n");
				bufferevent_write( bev, session->rtspinc.c_str(), p + 2 );
				bufferevent_write( bev, "\r\n", 2);
				bufferevent_write( bev, session->rtspinc.c_str() + (p + 4), (session->rtspinc.size() - p - 4));
				//bufferevent_write( bev, "\r\n", 2);
			}
			else
			{
				bufferevent_write( bev, session->rtspinc.c_str(), session->rtspinc.size() );
				bufferevent_write( bev, "\r\n", 2);
			}

			printf("OUT>>\n%s\n", session->rtspinc.c_str());
			
			session->rtspinc.clear();
		}
	}
}

void 
MediaSession::Send(struct bufferevent *bev, void *ctx)
{
	MediaSession *session = (MediaSession *)ctx;
	//printf("Send OK\n");
	//session->rtspinc.clear();
}



MediaSession::~MediaSession()
{

}

