#include "stdafx.h"
#include "h264MediaStream.h"


h264MediaStream::h264MediaStream()
{
	
}

h264MediaStream::~h264MediaStream()
{

}

void h264MediaStream::DevNode(const Buffer& buf, unsigned pos)
{
	unsigned sta = pos;
	const char* pdata = buf.GetBuffer();

	bool NalFlag = false;
	
	for (;sta < buf.GetMtuValue() - 4; sta ++)
	{		
		if (pdata[sta] == 0x00 && pdata[sta + 1] == 0x00 && 
			(pdata[sta + 2] ==0x01 || (pdata[sta + 3] == 0x00 && pdata[sta + 4] == 0x01)))
		{
			//NALÍ·»òÎ²
			if (NalFlag)
			{
				NalFlag = true;
				sta += (pdata[sta + 3]==0?2:3);
			}
			else
			{
				Buffer *NewNode = new Buffer(sta - pos);
				NewNode->FullBuffer(buf, pos, sta - pos -1);
				bufferlist.push(NewNode);
				pos += sta - pos + (pdata[sta + 3]==0?2:3);
				sta += (pdata[sta + 3]==0?2:3);
			}
		}
	}
	//return pos;
}

Buffer* h264MediaStream::GetNode()
{
	if (bufferlist.size())
	{
		Buffer* ret = bufferlist.front();
		bufferlist.pop();
		return ret;
	}
	return NULL;
}

void h264MediaStream::PullNode(const Buffer& Pulldata)
{
	
}





