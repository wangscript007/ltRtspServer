#pragma once
#include "MediaBuffer.h"
class RtpTrans
{
public:
	RtpTrans();
	~RtpTrans();
protected:
    void createRtpPacket(unsigned char playType, int timeStamp, int ssrc);
private:
	MediaBuffer *MediaNode;
	unsigned short SeqNumber;
};

RtpTrans::RtpTrans()
{
}

RtpTrans::~RtpTrans()
{
}
