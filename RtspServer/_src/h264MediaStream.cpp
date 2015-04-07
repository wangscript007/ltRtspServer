#include "stdhead.h"
#include "h264MediaStream.h"
#include "BaseEncoder.h"

h264MediaStream::h264MediaStream()
{
	DevCount = 0;
	StreamSps = NULL;
}

h264MediaStream::~h264MediaStream()
{
	printf("Release H264 Buffer\n");
	delete StreamSps;
}

bool 
h264MediaStream::h264_decode_hrd_parameters(bs_t& s, h264_sps_t* p_sps)
{
	int cpb_count, i;
	cpb_count = bs_read_ue(&s) + 1;
	bs_read(&s, 4); /* bit_rate_scale */
	bs_read(&s, 4); /* cpb_size_scale */
	for(i=0; i<cpb_count; i++){
		bs_read_ue(&s); /* bit_rate_value_minus1 */
		bs_read_ue(&s); /* cpb_size_value_minus1 */
		bs_read(&s, 1);     /* cbr_flag */
	}
	bs_read(&s, 5); /* initial_cpb_removal_delay_length_minus1 */
	bs_read(&s, 5); /* cpb_removal_delay_length_minus1 */
	bs_read(&s, 5); /* dpb_output_delay_length_minus1 */
	bs_read(&s, 5); /* time_offset_length */
	return true;
}

bool 
h264MediaStream::ParseSqs(const buf_share_ptr SqsNode)
{
	assert(SqsNode);

	if (StreamSps == NULL)
		StreamSps = new h264_sps_t;
	uint8_t *pb_dec = NULL;
	int     i_dec = 0;
	bs_t s;
	int i_sps_id;
	i_dec = SqsNode->GetSizeValue(); 
	int nal_hrd_parameters_present_flag, vcl_hrd_parameters_present_flag;
	pb_dec = const_cast<uint8_t *>(SqsNode->GetBuffer());
	uint8_t *p_nal = new uint8_t[i_dec];
	
	for (int tmp = 1; tmp < i_dec ; tmp++)
	{
		if (tmp < i_dec - 3 && *(pb_dec + tmp) == 0x00 && *(pb_dec + tmp + 1) == 0x00 && *(pb_dec + tmp + 2) == 0x03)
		{
			*(p_nal + tmp - 1) = 0x00;
			*(p_nal + tmp) = 0x00;
			tmp += 3;
			continue;
		}
		*(p_nal + tmp - 1) = *(pb_dec + tmp);
	}

	
	pb_dec = p_nal;

	

	

	bs_init( &s, pb_dec, i_dec );
	// profile(8)
	StreamSps->profile_idc = bs_read( &s, 8);

	/* constraint_set012, reserver(5), level(8) */
	bs_skip( &s, 1+1+1 + 5 + 8 );
	/* sps id */
	i_sps_id = bs_read_ue( &s );
	if( i_sps_id >= 32/*SPS_MAX*/ )
	{
		printf("invalid SPS (sps_id=%d)", i_sps_id );
		delete pb_dec;
		return false;
	}

	StreamSps->scaling_matrix_present = 0;
	if(StreamSps->profile_idc >= 100)		//high profile
	{ 
		if(bs_read_ue(&s) == 3)			//chroma_format_idc
			bs_read(&s, 1);				//residual_color_transform_flag
		bs_read_ue(&s);					//bit_depth_luma_minus8
		bs_read_ue(&s);					//bit_depth_chroma_minus8
		StreamSps->transform_bypass = bs_read(&s, 1);
		bs_skip(&s, 1); //decode_scaling_matrices(h, sps, NULL, 1, sps->scaling_matrix4, sps->scaling_matrix8);
	}

	/* Skip i_log2_max_frame_num */
	StreamSps->log2_max_frame_num = bs_read_ue( &s );
	if( StreamSps->log2_max_frame_num > 12)
		StreamSps->log2_max_frame_num = 12;
	/* Read poc_type */
	StreamSps->poc_type/*->i_pic_order_cnt_type*/ = bs_read_ue( &s );
	if( StreamSps->poc_type == 0 )
	{
		/* skip i_log2_max_poc_lsb */
		StreamSps->log2_max_poc_lsb/*->i_log2_max_pic_order_cnt_lsb*/ = bs_read_ue( &s );
		if( StreamSps->log2_max_poc_lsb > 12 )
			StreamSps->log2_max_poc_lsb = 12;
	}
	else if( StreamSps->poc_type/*p_sys->i_pic_order_cnt_type*/ == 1 )
	{
		int i_cycle;
		/* skip b_delta_pic_order_always_zero */
		StreamSps->delta_pic_order_always_zero_flag/*->i_delta_pic_order_always_zero_flag*/ = bs_read( &s, 1 );
		/* skip i_offset_for_non_ref_pic */
		bs_read_se( &s );
		/* skip i_offset_for_top_to_bottom_field */
		bs_read_se( &s );
		/* read i_num_ref_frames_in_poc_cycle */
		i_cycle = bs_read_ue( &s );
		if( i_cycle > 256 ) i_cycle = 256;
		while( i_cycle > 0 )
		{
			/* skip i_offset_for_ref_frame */
			bs_read_se(&s );
			i_cycle--;
		}
	}
	/* i_num_ref_frames */
	bs_read_ue( &s );
	/* b_gaps_in_frame_num_value_allowed */
	bs_skip( &s, 1 );

	/* Read size */
	StreamSps->mb_width/*->fmt_out.video.i_width*/  = 16 * ( bs_read_ue( &s ) + 1 );
	StreamSps->mb_height/*fmt_out.video.i_height*/ = 16 * ( bs_read_ue( &s ) + 1 );

	/* b_frame_mbs_only */
	StreamSps->frame_mbs_only_flag/*->b_frame_mbs_only*/ = bs_read( &s, 1 );
	if( StreamSps->frame_mbs_only_flag == 0 )
	{
		bs_skip( &s, 1 );
	}
	/* b_direct8x8_inference */
	bs_skip( &s, 1 );

	/* crop */
	StreamSps->crop = bs_read( &s, 1 );
	if( StreamSps->crop )
	{
		/* left */
		bs_read_ue( &s );
		/* right */
		bs_read_ue( &s );
		/* top */
		bs_read_ue( &s );
		/* bottom */
		bs_read_ue( &s );
	}

	/* vui */
	StreamSps->vui_parameters_present_flag = bs_read( &s, 1 );
	if( StreamSps->vui_parameters_present_flag )
	{
		int aspect_ratio_info_present_flag = bs_read( &s, 1 );
		if( aspect_ratio_info_present_flag )
		{
			static const struct { int num, den; } sar[17] =
			{
				{ 0,   0 }, { 1,   1 }, { 12, 11 }, { 10, 11 },
				{ 16, 11 }, { 40, 33 }, { 24, 11 }, { 20, 11 },
				{ 32, 11 }, { 80, 33 }, { 18, 11 }, { 15, 11 },
				{ 64, 33 }, { 160,99 }, {  4,  3 }, {  3,  2 },
				{  2,  1 },
			};

			int i_sar = bs_read( &s, 8 );

			if( i_sar < 17 )
			{
				StreamSps->sar.num = sar[i_sar].num;
				StreamSps->sar.den = sar[i_sar].den;
			}
			else if( i_sar == 255 )
			{
				StreamSps->sar.num = bs_read( &s, 16 );
				StreamSps->sar.den = bs_read( &s, 16 );
			}
			else
			{
				StreamSps->sar.num = 0;
				StreamSps->sar.den = 0;
			}

			//if( den != 0 )
			//	p_dec->fmt_out.video.i_aspect = (int64_t)VOUT_ASPECT_FACTOR *
			//	( num * p_dec->fmt_out.video.i_width ) /
			//	( den * p_dec->fmt_out.video.i_height);
			//else
			//	p_dec->fmt_out.video.i_aspect = VOUT_ASPECT_FACTOR;
		}
		else
		{
			StreamSps->sar.num = 0;
			StreamSps->sar.den = 0;
		}

		if(bs_read(&s, 1))		/* overscan_info_present_flag */
		{
			bs_read(&s, 1);     /* overscan_appropriate_flag */
		}

		if(bs_read(&s, 1))		/* video_signal_type_present_flag */
		{      
			bs_read(&s, 3);		/* video_format */
			bs_read(&s, 1);     /* video_full_range_flag */

			if(bs_read(&s, 1))  /* colour_description_present_flag */
			{
				bs_read(&s, 8);	/* colour_primaries */
				bs_read(&s, 8); /* transfer_characteristics */
				bs_read(&s, 8); /* matrix_coefficients */
			}
		}

		if(bs_read(&s, 1))		/* chroma_location_info_present_flag */
		{
			bs_read_ue(&s);		/* chroma_sample_location_type_top_field */
			bs_read_ue(&s);		/* chroma_sample_location_type_bottom_field */
		}

		StreamSps->timing_info_present_flag = bs_read(&s, 1);
		if(StreamSps->timing_info_present_flag)
		{
			StreamSps->num_units_in_tick = bs_read(&s, 32);
			StreamSps->time_scale = bs_read(&s, 32);
			StreamSps->fixed_frame_rate_flag = bs_read(&s, 1);
		}

		nal_hrd_parameters_present_flag = bs_read(&s, 1);
		if(nal_hrd_parameters_present_flag)
			h264_decode_hrd_parameters(s, StreamSps);
		vcl_hrd_parameters_present_flag = bs_read(&s, 1);
		if(vcl_hrd_parameters_present_flag)
			h264_decode_hrd_parameters(s, StreamSps);
		if(nal_hrd_parameters_present_flag || vcl_hrd_parameters_present_flag)
			bs_read(&s, 1);     /* low_delay_hrd_flag */
		bs_read(&s, 1);         /* pic_struct_present_flag */

		StreamSps->bitstream_restriction_flag = bs_read(&s, 1);
		if(StreamSps->bitstream_restriction_flag)
		{
			unsigned int num_reorder_frames;
			bs_read(&s, 1);     /* motion_vectors_over_pic_boundaries_flag */
			bs_read_ue(&s); /* max_bytes_per_pic_denom */
			bs_read_ue(&s); /* max_bits_per_mb_denom */
			bs_read_ue(&s); /* log2_max_mv_length_horizontal */
			bs_read_ue(&s); /* log2_max_mv_length_vertical */
			num_reorder_frames= bs_read_ue(&s);
			bs_read_ue(&s); /*max_dec_frame_buffering*/

			if(num_reorder_frames > 16 /*max_dec_frame_buffering || max_dec_frame_buffering > 16*/){
				printf("illegal num_reorder_frames %d\n", num_reorder_frames);
				delete pb_dec;
				return true;
			}

			StreamSps->num_reorder_frames= num_reorder_frames;
		}
	}
	delete pb_dec;
	return true;
}

void
h264MediaStream::DevLargeNode(const Buffer& buf, unsigned pos, int length)
{
	unsigned sta = pos + length;
	const unsigned char* pdata = buf.GetBuffer();
	Buffer RtpHead(RTP_HEADSIZE);
	while (sta - pos > NETMTU - RTP_HEADSIZE)
	{
		DevCount ++;
		Buffer *NewNode = new Buffer(NETMTU);
		NewNode->FullBuffer(RtpHead, 0, RTP_HEADSIZE);
		NewNode->FullBuffer(buf, pos, NETMTU - RTP_HEADSIZE);
		NewNode->SetByte(DevCount, 0);
		pos += (NETMTU - RTP_HEADSIZE);
		bufferlist.push(NewNode);
	}
	Buffer *NewNode = new Buffer(sta - pos);
	NewNode->FullBuffer(RtpHead, 0, RTP_HEADSIZE);
	NewNode->FullBuffer(buf, pos, sta - pos);
	NewNode->SetByte(DevCount, 0);
	bufferlist.push(NewNode);
	DevCount = 0;
}

//分离出NALU（1），同时对0x00 0x00 0x03进行处理（3），处理之后将每个NALU单元插入队列中。
//int CountForPos = pos;
//for (int p = pos; p < sta; p++)
//{
//	//（2）分离出与Start Code和关键标志位重复的数据 如 0x00 0x00 0x03 0x01 为 0x00 0x00 0x01
//	if (p < sta - 3 && pdata[p] == 0x00 && pdata[p + 1] == 0x00 && pdata[p + 2] == 0x03 )
//	{
//		NewNode->FullBuffer(&pdata[CountForPos], p - CountForPos + 1);
//		p += 2;
//		CountForPos = p;
//	}
//}
//NewNode->FullBuffer(buf, CountForPos, sta - CountForPos);


void 
h264MediaStream::DevNode(const Buffer& buf, unsigned pos)
{
	unsigned sta = pos;
	const unsigned char* pdata = buf.GetBuffer();

	bool NalFlag = false;
	
	for (;sta < buf.GetSizeValue() - 4; sta ++)
	{		
		//（1）判断Start Code
		if (pdata[sta] == 0x00 && pdata[sta + 1] == 0x00 && 
			(pdata[sta + 2] ==0x01 || (pdata[sta + 2] == 0x00 && pdata[sta + 3] == 0x01)))
		{
			//NAL头或尾
			if (!NalFlag)
			{
				NalFlag = true;
				if (DevCount > 0)
				{
					DevLargeNode(buf, 0, sta - 1);
				}
				sta += (pdata[sta + 2]==0?3:2);
				pos += (sta + 1);
			}
			else
			{
				if ((pdata[pos] & 0x1f) >= 0x06)
				{
					Buffer *NewNode = new Buffer(sta - pos);
					NewNode->FullBuffer(buf, pos, sta - pos);
					bufferlist.push(NewNode);
					pos += sta - pos + (pdata[sta + 2]==0?4:3);
					sta += (pdata[sta + 2]==0?4:3);
					continue;
				}
				DevLargeNode(buf, pos, sta - pos);
				pos += sta - pos + (pdata[sta + 2]==0?4:3);
				sta += (pdata[sta + 2]==0?4:3);
				
			}
		}
	}
	DevLargeNode(buf, pos, buf.GetSizeValue() - pos);
	
}

bool 
h264MediaStream::h264Base64Ps(buf_share_ptr buf, std::string& psbase)
{
	assert(buf);
	unsigned short t = buf->GetPosValue();
	BaseEncoder::Base64Encode((PBYTE)buf->GetBuffer(), buf->GetSizeValue(), &psbase);
	return true;
}

buf_share_ptr 
h264MediaStream::GetNode()
{
	if (bufferlist.size())
	{
		Buffer* ret = bufferlist.front();
		bufferlist.pop();
		buf_share_ptr retptr(ret);
		return retptr;
	}
	return NULL;
}

void 
h264MediaStream::PullNode(const Buffer& Pulldata)
{
	
}

uint8_t 
h264MediaStream::GetNalType(buf_share_ptr buf)
{
	if (buf->GetByte(0) < 50)
	{
		return (buf->GetByte(RTP_HEADSIZE)&0x1f);
	}
	return (buf->GetByte(0)&0x1f);
	
}




