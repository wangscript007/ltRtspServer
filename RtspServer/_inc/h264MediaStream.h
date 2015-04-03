#pragma once
#include <vector>
#include <queue>
#include "MediaBuffer.h"
#include "MediaStream.h"
#include "bit/vlc_bits.h"


using namespace std;

#define MAXDATABUF 10
#define BUFFERSIZE 1024*1024
class h264MediaStream: public MediaStream
{
public:
	h264MediaStream();
	virtual ~h264MediaStream();
	void DevNode(const Buffer& buf, unsigned pos);
	void h264_decode_annexb(Buffer *buf);

	virtual Buffer * GetNode();
	virtual void PullNode(const Buffer& Pulldata);

	uint8_t GetNalType(uint8_t queueIndex);

protected:
	//queue
	
	Buffer *OwnBuffer;
private:
};