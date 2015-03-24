#include "../_inc/stdhead.h"
#include "../_inc/RtpTrans.h"

/*
0                   1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|V=2|P|X|  CC   |M|     PT      |       sequence number         |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                           timestamp                           |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|           synchronization source (SSRC) identifier            |
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
|            contributing source (CSRC) identifiers             |
|                             ....                              |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

void RtpTrans::createRtpPacket(unsigned char playType, int timeStamp, int ssrc)
{
	//version 10  no padding 0
	unsigned char header[16];
	header[0] = 0x80;
	//M playtype
	header[1] = playType;
	header[2] = (SeqNumber >> 8) & 0xff;
	header[3] = SeqNumber & 0xff;

	header[4] = (timeStamp >> 24) & 0xff;
	header[5] = (timeStamp >> 16) & 0xff;
	header[6] = (timeStamp >> 8)  & 0xff;
	header[7] = timeStamp & 0xff;

	header[8] = (ssrc >> 24) & 0xff;
	header[9] = (ssrc >> 16) & 0xff;
	header[10] = (ssrc >> 8)  & 0xff;
	header[11] = ssrc & 0xff;
}
