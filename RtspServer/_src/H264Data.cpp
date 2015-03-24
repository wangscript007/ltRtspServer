#include "../_inc/stdhead.h"
#include "../_inc/H264Data.h"


H264Data::H264Data()
{
	
}

unsigned H264Data::FindNal(const Buffer& buf, unsigned pos)
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
				sta += pdata[sta + 3]==0?2:3;
			}
			else
			{
				Buffer *NewNode = new Buffer(sta - pos);
				NewNode->FullBuffer(buf, pos, sta - pos -1);
				bufferlist.push_back(NewNode);

				pos += sta - pos + (pdata[sta + 3]==0?2:3);
				sta += (pdata[sta + 3]==0?2:3);
			}
		}
	}
	return pos;
}

Buffer* H264Data::GetNal()
{
	if (bufferlist.size())
	{
		return bufferlist.at(0);
	}

	return NULL;
}

void H264Data::DealData()
{
	
}





