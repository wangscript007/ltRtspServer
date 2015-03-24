#include "../_inc/stdhead.h"
#include "../_inc/MediaBuffer.h"


Buffer::Buffer(unsigned mtu /* = 1480 */)
{
	MTU = mtu;
	pbuffer = new char[mtu];

	memset(pbuffer, 0, mtu);
	
	pos = mtu;
}

Buffer::Buffer(const Buffer& ibuffer)
{
	MTU = ibuffer.MTU;
	pbuffer = new char[MTU];
	memset(pbuffer, 0, MTU);
	memcpy(pbuffer, ibuffer.pbuffer, MTU);
	pos = ibuffer.pos;
}


Buffer::~Buffer()
{
	delete[] pbuffer;
};

unsigned Buffer::GetMtuValue() const
{
	return MTU;
}

unsigned Buffer::GetPosValue() const
{
	return pos;
}

bool Buffer::Clear()
{
	memset(pbuffer, 0, pos);
	pos = MTU;
	return true;
}

bool Buffer::FullBuffer(const char* buffer, int lenth)
{
	assert(buffer&&lenth);
	if (pos > lenth)
	{
		memcpy(&pbuffer[pos], buffer, lenth);
		pos -= lenth;
		return true;
	}
	return false;
}
bool Buffer::FullBuffer(const Buffer& buf,unsigned start,unsigned length)
{
	if (this == &buf)
	{
		return true;
	}
	if (pos >length)
	{
		memcpy(&pbuffer[MTU - pos], &buf.pbuffer[start], length);
		pos -= length;
		return true;
	}
	return false;
}

const char* Buffer::GetBuffer()const
{
	return pbuffer;
}

const Buffer& Buffer::operator=(const Buffer& ibuffer)
{
	if (this != &ibuffer)
	{
		MTU = ibuffer.MTU;
		delete[] pbuffer;
		pbuffer = new char[MTU];
		memset(pbuffer, 0, MTU);
		if (FullBuffer(ibuffer.pbuffer, ibuffer.pos))
		{
			pos = ibuffer.pos;
			return *this;
		}
		//需要一个log
		pos = MTU;
		return *this;
	}
	return *this;
}

void Buffer::SetByte(unsigned char byteSet, unsigned pos)
{
	pbuffer[pos] = byteSet;
}

unsigned char Buffer::GetByte(unsigned pos)
{
	return pbuffer[pos];
}










MediaBuffer::~MediaBuffer()
{

}




