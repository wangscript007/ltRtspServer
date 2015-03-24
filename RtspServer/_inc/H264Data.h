#pragma once
#include <vector>
#include "MediaBuffer.h"



using namespace std;

#define MAXDATABUF 10
#define BUFFERSIZE 1024*1024
class H264Data
{
public:
	H264Data();
	~H264Data();
	void DealData();
	unsigned FindNal(const Buffer& buf, unsigned pos);
	Buffer * GetNal();
protected:
	//queue
	vector<Buffer *> bufferlist;
	Buffer *OwnBuffer;
private:
};