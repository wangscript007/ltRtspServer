#pragma once
#include <iostream>

enum BUFLOG
{
	DONE,		//����
	NONEDATA,	//��
	HALFSIZE,	//������ֵ
	HAVEDATA,   //�Ѵ�������
	FULLBUFFER,	//�����Ѵ������ݵ�Ԫ������һ����Ԫ����ʲ����
	LARGEPACKET,//�������ݵ�Ԫ�ϴ󣬴���MTU��Ҫ�ְ�
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



