#pragma once
#include <iostream>

enum BUFLOG
{
	DONE,		//正常
	NONEDATA,	//空
	HALFSIZE,	//到达阈值
	HAVEDATA,   //已存在数据
	FULLBUFFER,	//用于已存在数据单元，但下一个单元过大故不填充
	LARGEPACKET,//用于数据单元较大，大于MTU需要分包
};

class Buffer
{
public:
	explicit Buffer(unsigned mtu = 1480);
	Buffer(const Buffer& ibuffer);
	
	const Buffer& operator=(const Buffer& ibuffer);

	unsigned GetMtuValue() const;
	unsigned GetPosValue() const;
	const char* GetBuffer() const;
	bool Clear();
	void SetByte(unsigned char byteSet, unsigned pos);
	unsigned char GetByte(unsigned pos);
	inline bool FullBuffer(const char* buffer,int lenth);
	bool FullBuffer(const Buffer& buf,unsigned start,unsigned length);
protected:
	virtual ~Buffer();

private:
	char *pbuffer;
	unsigned short pos;
	unsigned MTU;
};

class MediaBuffer : public Buffer
{
public:
	explicit MediaBuffer(unsigned mtu):Buffer(mtu){}
	//virtual bool PutInBuffer(const char* buffer,int length);

protected:
	virtual ~MediaBuffer();
	BUFLOG log;
};



