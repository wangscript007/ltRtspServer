#pragma once

#include <iostream>

enum rtsp_type
{
	ERRORTYPE,
	OPTIONS,
	DESCRIBE,
	SETUP,
	PLAY,
	PAUSE,
	TEARDOWN
};

typedef struct _rtspinfo
{
	rtsp_type	type;
	std::string	addr;
	std::string file_path;
	std::string	cseq;
	unsigned int session;
}rtspinfo;

//���ڴ���Rtsp����
class rtsp_string
{
public:
	explicit rtsp_string();
	
	const rtspinfo& deal_requset(std::string& info);
protected:
	void deal_string(std::string& info);
	bool create_sdp(std::string& info);
	
	bool deal_options(std::string& info);
	bool deal_describe(std::string& info);
	bool deal_setup(std::string& info);
	bool deal_play(std::string& info);
	bool deal_pause(std::string& info);
	bool deal_teardown(std::string& info);

	unsigned get_addr(std::string& info,unsigned pos);
	unsigned get_filepath(std::string& info,unsigned pos);
	unsigned get_cseq(std::string& info,unsigned pos);
	
private:

	~rtsp_string();
	//std::string sdpstring;

	rtspinfo deal_info;

};