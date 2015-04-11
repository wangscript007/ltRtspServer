#include "stdhead.h"
#include "MediaStream.h"


MediaStream::MediaStream()
{

}

MediaStream::~MediaStream()
{

}

void MediaStream::AddNode()
{

}

void MediaStream::DevNode(const Buffer& buf, unsigned pos)
{

}

buf_share_ptr MediaStream::GetNode()
{
    buf_share_ptr p(new Buffer(1));
    return p;
}


const unsigned MediaStream::GetListLen()const
{
	return bufferlist.size();
}

