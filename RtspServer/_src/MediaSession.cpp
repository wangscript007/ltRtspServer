#include "stdhead.h"
#include "MediaSession.h"


long long
MediaSession::GenSessionID()
{
    /*
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
    */
    struct timeval start;
    gettimeofday(&start, NULL);
    long long value = start.tv_sec<<32|start.tv_usec;

    return (long long)value;
}

void 
MediaSession::SetDescribe()
{
	unsigned p = 0;
	char SessionId[48] = {0};
	sessionid = GenSessionID();
    snprintf(SessionId, 48, "%llu", sessionid);
    printf("\nValue Of Session: %llu\n", sessionid);
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
MediaSession::DealRtsp(struct bufferevent* bev)
{
    char pstring[1480] = {0};

    if(bev && bufferevent_read(bev, pstring, 1480))
    {
        //printf("Get New Node:\n%s\n", pstring);
        rtspinc.append(pstring);
        if ( rtspinc.find("\r\n\r\n") != std::string::npos )
        {
            printf("IN<<\n%s\n", rtspinc.c_str());

            if(rs.deal_requset(rtspinc).type == DESCRIBE)
            {
                SetDescribe();
                int p = rtspinc.find("\r\n\r\n");
                bufferevent_write( bev, rtspinc.c_str(), p + 2 );
                bufferevent_write( bev, "\r\n", 2);
                bufferevent_write( bev, rtspinc.c_str() + (p + 4), (rtspinc.size() - p - 4));
                //bufferevent_write( bev, "\r\n", 2);
            }
            else
            {
                bufferevent_write( bev, rtspinc.c_str(), rtspinc.size() );
                bufferevent_write( bev, "\r\n", 2);
            }
            printf("OUT>>\n%s\n", rtspinc.c_str());
            rtspinc.clear();
        }
    }
}

void
MediaSession::Recv(evutil_socket_t fd, short events, void *arg)
{
    char tmp[64] = {0};
    recv(fd, tmp, 64, 0);
    TWork* work = (TWork *)arg;
    //printf("PID : %d\n", work->pid);
    PQNode node = MesQueue::GetInstance()->OutQueue();
    while (node != NULL)
    {

        if (node->type == TCP && node->events == EV_READ)
        {
            assert(node->forbev && node->arg);
            MediaSession* ses = (MediaSession *)node->arg;
            bufferevent_lock((struct bufferevent *)node->forbev);
            ses->DealRtsp((struct bufferevent *)node->forbev);
            bufferevent_unlock((struct bufferevent *)node->forbev);
            delete node;
        }
        node = MesQueue::GetInstance()->OutQueue();
    }
}

void
MediaSession::Send(evutil_socket_t fd, short events, void *arg)
{
    //MediaSession *session = (MediaSession *)ctx;
    //printf("Send OK\n");
    //session->rtspinc.clear();
}


MediaSession::~MediaSession()
{

}

