/*
 * Copyright (c) 2016-2022 The Khronos Group Inc.
 * Copyright 2022, QNX Software Systems.
 * Copyright (C) 2022 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "omax_dec_priv.h"

enum {
    NAL_UNSPECIFIED = 0, /* Derived from the definition described in P.48 of "T-REC-H.264-200305-S!!PDF-E.pdf" */
    NAL_SLICE = 1,
    NAL_DPA,
    NAL_DPB,
    NAL_DPC,
    NAL_IDR_SLICE,
    NAL_SEI,
    NAL_SPS,
    NAL_PPS,
    NAL_AUD,
    NAL_END_SEQUENCE,
    NAL_END_STREAM,
    NAL_FILLER_DATA,
};

/* The following tables are required to parse the AVCC SPS block to retrieve height and
 * width for H.264 streams.
 */
static const uint8_t vlc_len[512]={
14,13,12,12,11,11,11,11,10,10,10,10,10,10,10,10,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,3,3,3,3,3,
3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1};

static const uint8_t golomb_vals[512]={
31,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,
7,7,7,7,8,8,8,8,9,9,9,9,10,10,10,10,11,11,11,11,12,12,12,12,13,13,13,13,14,14,14,14,3,3,3,3,3,3,
3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,
6,6,6,6,6,6,6,6,6,6,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0};

static const int8_t signed_golomb_vals[512]={
16,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,8,-8,9,-9,10,-10,11,-11,12,-12,13,-13,14,-14,15,
-15,4,4,4,4,-4,-4,-4,-4,5,5,5,5,-5,-5,-5,-5,6,6,6,6,-6,-6,-6,-6,7,7,7,7,-7,-7,-7,-7,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,2,2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
3,3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0};

static const uint8_t h264_aspec_ratio_from_idc[14][2] = {
		{1, 1},
		{1, 1},
		{12, 11},
		{10, 11},
		{16, 11},
		{40, 33},
		{24, 11},
		{20, 11},
		{32, 11},
		{80, 33},
		{18, 11},
		{15, 11},
		{64, 33},
		{160, 99}
};

static inline uint32_t BS_BYTE_READ(const uint8_t *x)
{
    return (uint32_t)((((x))[0] << 24) | (((x))[1] << 16) | (((x))[2] <<  8) | ((x))[3]);
}

static inline int32_t mmf_log2(uint32_t v)
{
	int32_t log2 = 0;
	int32_t i;

	for (i = 31; i >= 0; i--)
    {
		if (v >> i)
        {
			log2 = i;
			break;
		}
	}
	return log2;
}

static inline uint32_t mmf_bitstremgetbit(psbitstream_t *s)
{
	int32_t byte    = s->bit_offset / 8;
	uint8_t result;

	if (byte >= (int32_t)s->buff_size)
    {
		return 0;
	}
	result  = s->buff[ byte ];
	result <<= (s->bit_offset & 0x07);
	result >>= 7;
	s->bit_offset++;

	return result;
}

/* This is lazy, but we're not worried about performance. */
static inline uint32_t mmf_bitstremgetbits(psbitstream_t *s, int n)
{
	int32_t  i;
	uint32_t retval = 0;

	if (s->bit_offset/8 >= (int32_t)s->buff_size)
    {
		return 0;
	}

	n--;
	for (i = 0; i < n ; i++)
    {
		retval |= mmf_bitstremgetbit(s);
		retval <<= 1;
	}
	retval |= mmf_bitstremgetbit(s);

	return retval;
}

/* These are exp golomb code reading funcs. Required for AVC/ H.264 */
static uint32_t mmf_golombunsigned(psbitstream_t *psb)
{
	uint32_t retval = 0;
	int32_t  byte = psb->bit_offset / 8;
	int32_t  log;
	int32_t  bytesleft = psb->buff_size - byte;

	if (psb->bit_offset/8 >= (int32_t)psb->buff_size)
    {
		return 0;
	}

	if (bytesleft >= 4)
    {
		retval = BS_BYTE_READ(&psb->buff[byte]);
	}
    else
    {
		uint8_t *buffptr = (uint8_t*)&psb->buff[byte];
		OMAX_BS_PART_READ(buffptr, retval, bytesleft);
	}

	retval <<= (psb->bit_offset & 0x07);

	if(retval >= OMAX_SECOND_BYTE)
    {
		retval >>= 23;
		psb->bit_offset += vlc_len[retval];

		return golomb_vals[retval];
	}
	log = 2 * mmf_log2(retval) - 31;
	psb->bit_offset += 32 - log;

	retval >>= log;
	retval--;
	return retval;
}

static int32_t mmf_golombconstrained(psbitstream_t *psb)
{
	uint32_t retval = 0;
	int32_t  byte = psb->bit_offset / 8;
	int32_t  bytesleft = psb->buff_size - byte;

	if (psb->bit_offset/8 >= (int32_t)psb->buff_size)
    {
		return 0;
	}

	if (bytesleft >= 4)
    {
		retval = BS_BYTE_READ(&psb->buff[byte]);
	}
    else
    {
		uint8_t *buffptr = (uint8_t*)&psb->buff[byte];
		OMAX_BS_PART_READ(buffptr, retval, bytesleft);
	}
	retval <<= (psb->bit_offset & 0x07);
	retval >>= 23;
	psb->bit_offset += vlc_len[retval];

	return golomb_vals[retval];
}

static int32_t mmf_golombsigned(psbitstream_t *psb)
{
	int32_t  retval = 0;
	uint32_t codenum = 0;
	int32_t  byte = psb->bit_offset / 8;
	int32_t  log;
	int32_t  bytesleft = psb->buff_size - byte;

	if (psb->bit_offset/8 >= (int32_t)psb->buff_size)
    {
		return 0;
	}

	if (bytesleft >= 4)
    {
		codenum = BS_BYTE_READ(&psb->buff[byte]);
	}
    else
    {
		uint8_t *buffptr = (uint8_t*)&psb->buff[byte];
		OMAX_BS_PART_READ(buffptr, codenum, bytesleft);
	}
	codenum <<= (psb->bit_offset & 0x07);

	if(codenum >= OMAX_SECOND_BYTE)
    {
		codenum >>= 23;
		psb->bit_offset += vlc_len[codenum];

		return signed_golomb_vals[codenum];
	}
	log = 2 * mmf_log2(codenum) - 31;
	psb->bit_offset += 32 - log;

	codenum >>= log;
	if (codenum & 0x00001)
    {
		retval = -((int32_t)(codenum>>1));
	}
    else
    {
		retval =  (codenum>>1);
	}

	return retval;
}

static void skip_list(psbitstream_t *psb, int size)
{
	/* We only worry about it if the presence bit says it's there. */
	if(mmf_bitstremgetbit(psb))
    {
		int32_t val;
		int32_t i;

		val = 8;
		for(i = 0; i < size; i++)
        {
			if (val)
            {
				val = (val + mmf_golombsigned(psb)) & 0xff;
			}
			/* If it's not there */
			if(!i && !val)
            {
				break;
			}
		}
	}
}

static int h264_parse_sps(const uint8_t *rawps, uint32_t pslen, decoder_info_t *info, avc1_decoder_specific_t *avc1)
{
	psbitstream_t   psb;
	int32_t         profile;
	int32_t         poc_type;
	int32_t         i;
	int32_t         ar_idc;

	/* Just in case there is no aspect ratio information in the SPS. */
	info->par_x = 1;
	info->par_y = 1;
	info->width = 0;
	info->height = 0;

	/* Turn our buffer into a bitstream. */
	psb.bit_offset = 0;
	psb.buff_size = pslen;
	psb.buff = rawps;

	profile = mmf_bitstremgetbits(&psb, 8);
	avc1->profile_indication    = profile;
	avc1->profile_compatibility = mmf_bitstremgetbits(&psb, 8);
	avc1->level_indication      = mmf_bitstremgetbits(&psb, 8);

	if(mmf_golombconstrained(&psb) >= 32)
    {
		return -1;
	}

	/* More to do for high profile or better. */
    if(profile == 100 || profile == 110 ||
            profile == 122 || profile == 244 || profile == 44 ||
            profile == 83 || profile == 86 || profile == 118 ||
            profile == 128 || profile == 138 || profile == 139 ||
            profile == 134 || profile == 135)
    {

		avc1->chroma_format_idc = mmf_golombconstrained(&psb);
		if (avc1->chroma_format_idc == 3)
		{
            mmf_bitstremgetbit(&psb);
        }

		avc1->bit_depth_luma = mmf_golombunsigned(&psb) + 8;
		avc1->bit_depth_chroma = mmf_golombunsigned(&psb) + 8;
		mmf_bitstremgetbit(&psb);

		if (mmf_bitstremgetbit(&psb))
        {
			for (i = 0; i < 6; i++)
            {
				skip_list(&psb, 16);
			}
			for (i = 0; i < 2; i++)
            {
				skip_list(&psb, 64);
			}
		}
	}
    else
    {
        /* set default value if they are absent */
        avc1->chroma_format_idc = 1;
        avc1->bit_depth_luma = 8;
        avc1->bit_depth_chroma = 8;
    }

	mmf_golombunsigned(&psb);
	poc_type = mmf_golombconstrained(&psb);

	if (poc_type == 0)
    {
		mmf_golombunsigned(&psb);
	}
    else if (poc_type == 1)
    {
		int32_t cycle_len;

		mmf_bitstremgetbit(&psb);
		mmf_golombsigned(&psb);
		mmf_golombsigned(&psb);
		cycle_len = mmf_golombunsigned(&psb);

		for(i = 0; i < cycle_len; i++)
        {
            mmf_golombsigned(&psb);
        }
	}

	avc1->num_ref_frames =  mmf_golombconstrained(&psb);
	mmf_bitstremgetbit(&psb);

	/* Store the width and height */
	info->width = (mmf_golombunsigned(&psb) + 1) * 16;
    int32_t pic_height_in_map_units_minus1 = mmf_golombunsigned(&psb);

	/* Mine for aspect ratio info. */
	/* frame mbs only flag */
	if(mmf_bitstremgetbit(&psb) == 0)
    {
		avc1->frame_mbs_only_flag = 0;
		/* mb_adaptive_frame_field_flag */
		mmf_bitstremgetbit(&psb);
	}
    else
    {
		avc1->frame_mbs_only_flag = 1;
	}

	info->height = (pic_height_in_map_units_minus1 + 1) * (avc1->frame_mbs_only_flag ? 1 : 2) * 16;

	/* direct_8x8_inference_flag */
	mmf_bitstremgetbit(&psb);
	/* frame_cropping_flag */
	if(mmf_bitstremgetbit(&psb))
    {
		int vsub   = (avc1->chroma_format_idc == 1) ? 1 : 0;
		int hsub   = (avc1->chroma_format_idc == 1 || avc1->chroma_format_idc == 2) ? 1 : 0;
		int step_x = 1 << hsub;
		int step_y = (2 - avc1->frame_mbs_only_flag) << vsub;

		/* frame_crop_left_offset */
		unsigned int crop_left = mmf_golombunsigned(&psb);
		/* frame_crop_right_offset */
		unsigned int crop_right = mmf_golombunsigned(&psb);
		/* frame_crop_top_offset */
		unsigned int crop_top = mmf_golombunsigned(&psb);
		/* frame_crop_bottom_offset */
		unsigned int crop_bottom = mmf_golombunsigned(&psb);

		if (crop_left  > (unsigned)INT32_MAX / 4 / step_x ||
				crop_right > (unsigned)INT32_MAX / 4 / step_x ||
				crop_top   > (unsigned)INT32_MAX / 4 / step_y ||
				crop_bottom> (unsigned)INT32_MAX / 4 / step_y ||
				(crop_left + crop_right) * step_x >= (uint32_t)info->width ||
				(crop_top  + crop_bottom) * step_y >= (uint32_t)info->height
				)
        {
			WRAPPER_PRINTF("\ncrop values invalid %d %d %d %d / %d %d\n", crop_left, crop_right, crop_top, crop_bottom, info->width, info->height);
			avc1->cropping.crop_left = 0;
			avc1->cropping.crop_right = 0;
			avc1->cropping.crop_top = 0;
			avc1->cropping.crop_bottom = 0;
		}
        else
        {
			avc1->cropping.crop_left   = crop_left   * step_x;
			avc1->cropping.crop_right  = crop_right  * step_x;
			avc1->cropping.crop_top   = crop_top    * step_y;
			avc1->cropping.crop_bottom= crop_bottom * step_y;
		}
	}

	/* VUI Parameters Flags -- Aspect ratio is here. */
	if(mmf_bitstremgetbit(&psb))
    {
		WRAPPER_PRINTF("\nVUI Parameters present");
		/* aspect_ratio_info_present_flag */
		if (mmf_bitstremgetbit(&psb))
        {
			WRAPPER_PRINTF("\naspect ratio present");
			ar_idc = mmf_bitstremgetbits(&psb, 8);
			WRAPPER_PRINTF("\n    ar_idc = %d", ar_idc);
			if (ar_idc == OMAX_H264_CUSTOM_ASPECT_RATIO)
            {
				info->par_x = mmf_bitstremgetbits(&psb, 16);
				info->par_y = mmf_bitstremgetbits(&psb, 16);
				WRAPPER_PRINTF("\n    sar x = %d, sar y = %d.", info->par_x, info->par_y);
			}
            else
            {
				if (ar_idc < 14)
                {
					/* Interpret the pre-defined field. */
					info->par_x = h264_aspec_ratio_from_idc[ar_idc][0];
					info->par_y = h264_aspec_ratio_from_idc[ar_idc][1];
				}
			}
			/* By spec. if par x or par y are 0, par is unspecified.  So back to 1:1 */
			if (info->par_x == 0 || info->par_y == 0)
            {
				WRAPPER_PRINTF("\n    Undefined aspect ratio.");
				info->par_x = 1;
				info->par_y = 1;
			}
		}

		/* overscan_info_present_flag */
		if (mmf_bitstremgetbit(&psb))
        {
			mmf_bitstremgetbit(&psb);
		}

		/* video_signal_type_present_flag */
		if (mmf_bitstremgetbit(&psb))
        {
			mmf_bitstremgetbits(&psb, 3);
			avc1->video_full_range_flag = mmf_bitstremgetbit(&psb);
			/* colour_description_present_flag */
			if (mmf_bitstremgetbit(&psb))
            {
				mmf_bitstremgetbits(&psb, 24);
			}
		}

		/* chroma_loc_info_present_flag */
		if (mmf_bitstremgetbit(&psb))
        {
			mmf_golombunsigned(&psb);
			mmf_golombunsigned(&psb);
		}

		/*timing_info_present_flag */
		if (mmf_bitstremgetbit(&psb))
        {
			info->num_units_in_tick = mmf_bitstremgetbits(&psb, 32);
			info->time_scale = mmf_bitstremgetbits(&psb, 32);
			info->fixed_frame_rate_flag = mmf_bitstremgetbit(&psb);
		}
	}

	return EOK;
}

/* cleanup the avc configuration object */
void h264_free_decoder_specific(avc1_decoder_specific_t* avcc)
{
	int32_t i;
	if (avcc)
    {
		for(i = 0; i < avcc->sps_count; i++)
        {
			free(avcc->sps[i].string);
			free(avcc->sps[i].nalu);
		}
		free(avcc->sps);
		free(avcc->sps_info);
		for(i = 0; i < avcc->pps_count; i++)
        {
			free(avcc->pps[i].string);
			free(avcc->pps[i].nalu);
		}
		free(avcc->pps);
		free(avcc);
	}
}

int32_t h264_parse_decoder_config(const uint8_t* raw_avcc, uint32_t avcc_size, decoder_info_t *info_out, avc1_decoder_specific_t** avcc_out)
{
    int32_t i;
    avc1_decoder_specific_t *avcc;
    uint8_t *buf;
    int32_t offsetSps;
    int32_t offsetPps;
    uint8_t nalType;
    uint32_t size;

    if (avcc_size < 9)
    {
        return ENOTSUP;
    }

    /* Search for SPS and PPS */
    offsetSps = offsetPps = -1;
    for (i = 0; i < (int32_t)avcc_size - 4; i++)
    {
        /* First byte is a don't care, but must be present. */
        if (raw_avcc[i] == 0 && raw_avcc[i+1] == 0 && raw_avcc[i+2] == 1)
        {
            nalType = raw_avcc[i+3] & 0x1F;
            if (nalType == 7)
            {
                WRAPPER_PRINTF("\nFound SPS at offset %d", i);
                offsetSps = i;
            }
            if (nalType == 8)
            {
                WRAPPER_PRINTF("\nFound PPS at offset %d", i);
                offsetPps = i;
            }
        }
        if ((offsetSps != -1) && (offsetPps != -1))
        {
            break;
        }
    }

    if (offsetSps < 0)
    {
        WRAPPER_PRINTF("\nNo SPS");
        return ENOTSUP;
    }
    if (offsetPps < 0)
    {
        WRAPPER_PRINTF("\nNo PPS");
        return ENOTSUP;
    }

    if ((avcc = (avc1_decoder_specific_t *)calloc(1, sizeof(avc1_decoder_specific_t))) == NULL)
    {
        return ENOMEM;
    }

    avcc->sps_count             = 1;
    avcc->sps                   = (avc1_param_set_t*)calloc(1, sizeof(avc1_param_set_t));
    avcc->sps_info              = (decoder_info_t*)calloc(1, sizeof(decoder_info_t));
    if (!avcc->sps || !avcc->sps_info)
    {
        h264_free_decoder_specific(avcc);
        return ENOMEM;
    }

    /* Parse required info out of the SPS.
     * Start parsing SPS header
     */
    buf =  (uint8_t *)raw_avcc + offsetSps + 4;
    size = avcc_size - (offsetSps + 4);
    if (size < 2)
    {
        h264_free_decoder_specific(avcc);
        return ENOTSUP;
    }

    h264_parse_sps(buf, size, &avcc->sps_info[0], avcc);

    avcc->pps_count             = 1;
    avcc->pps                   = (avc1_param_set_t*)calloc(1, sizeof(avc1_param_set_t)*avcc->pps_count);

    /* For h.264 there can be multiple param sets, for now take the first.
     * we are returning all of them, so a parser can choose the appropriate one.
     */
    if (info_out)
    {
        *info_out = avcc->sps_info[0];
    }

    if (avcc_out)
    {
        *avcc_out = avcc;
    }
    else
    {
        h264_free_decoder_specific(avcc);
    }
    return EOK;
}

static inline omxil_bool CheckStartCode(
    const uint8_t *sc,
    uint32_t scsize,
    const uint8_t *inbuf,
    uint32_t isize,
    const uint32_t *byte_to_apply_mask,
    const uint8_t *start_code_mask)
{
    uint32_t sc_index = 0;

    if(isize < scsize)
    {
        return omxil_false_e;
    }

    while(scsize)
    {
        if(byte_to_apply_mask && start_code_mask && (sc_index == *byte_to_apply_mask))
        {
            if(*sc != (*inbuf & *start_code_mask))
            {
                return omxil_false_e;
            }
        }
        else
        {
            if(*sc != *inbuf)
            {
                return omxil_false_e;
            }
        }
        scsize--;
        sc++;
        inbuf++;
        sc_index++;
    }

    return omxil_true_e;
}


static inline int FindStartCode(
    const uint8_t *sc,
    uint32_t scsize,
    const uint8_t *inbuf,
    uint32_t isize,
    const uint32_t *byte_to_apply_mask,
    const uint8_t *start_code_mask)
{
    int size = 0;

    while(isize)
    {
        if(CheckStartCode(sc, scsize, inbuf, isize, byte_to_apply_mask, start_code_mask))
        {
            break;
        }

        isize--;
        size++;
        inbuf++;
    }

    return size;
}

static int H264CheckNalType(const uint8_t *inbuf, int *sps_pps, int *frame_data)
{
    int nalType = 0;

    /* detect SPS/PPS/Frame data */
    nalType = ((*inbuf) & 0x1f);
    if((nalType == NAL_SPS) || (nalType == NAL_PPS))
    {
        *sps_pps = 1;
    }
    else if(nalType > 0 && nalType <= NAL_IDR_SLICE)
    {
        *frame_data = 1;
    }

    return nalType;
}

int32_t loadConfig(OmxilVideoEncDec_t *encH)
{
    static const uint8_t sc[] = {0x00, 0x00, 0x01};  /* start code */
    int initial_sc_offset = 0;
    int sps_pps_found = 0;
    int frame_found = 0;
    int nalSize;
    uint8_t *buf;
    uint32_t size;
    int32_t err = EOK;

    size = encH->large_input_buf_data_size;
    buf = encH->currPtr;
    encH->config_size = 0;

    WRAPPER_PRINTF("\n%s=> size=%d, buf=%p", __func__, size, buf);

    if(encH->large_input_buf_data_size < (int32_t)(DIM(sc)+ 2))
    {
        WRAPPER_PRINTF("\n%s=> reach the end of data", __func__);
        return EINVAL;
    }
    /* check if the buffer starts with the start code, else skip the bytes */
    initial_sc_offset = FindStartCode(sc, DIM(sc), buf, encH->large_input_buf_data_size, NULL, NULL);
    WRAPPER_PRINTF("\n%s=> start code was found at initial_sc_offset %d (skipping these bytes)", __func__, initial_sc_offset);
    if(initial_sc_offset > encH->large_input_buf_data_size)
    {
        WRAPPER_ERROR("\n%s=> Wrong input stream initial_sc_offset=%d, encH->large_input_buf_data_size=%d", __func__, initial_sc_offset, encH->large_input_buf_data_size);
        return EINVAL;
    }

    buf += initial_sc_offset;
    size -= initial_sc_offset;

    if (size < DIM(sc)+ 2)
    {
        WRAPPER_ERROR("\n%s=> Only frame header found(size=%d), skip", __func__, size);
        return EINVAL;
    }

    while((size >= DIM(sc)+ 2))
    {
        /* detect SPS/PPS/Frame data */
        sps_pps_found = 0;

    #ifdef WRAPPER_DEBUG_MODE
        int nal_type = H264CheckNalType((buf + DIM(sc)), &sps_pps_found, &frame_found);
    #else
        H264CheckNalType((buf + DIM(sc)), &sps_pps_found, &frame_found);
    #endif

        WRAPPER_PRINTF("\n%s=> nal_type=%d, sps_pps_found=%d", __func__, nal_type, sps_pps_found);
        /* if we've already started pushing NALU to the buffer, check
         * if this next NALU starts a new access unit
         */
        WRAPPER_PRINTF("\n%s=> %x %x %x %x %x", __func__, buf[0],buf[1],buf[2],buf[3],buf[4]);
        nalSize = DIM(sc) + FindStartCode(sc, DIM(sc), buf+DIM(sc), size-DIM(sc), NULL, NULL);
        WRAPPER_PRINTF("\n%s=> nalSize=%d", __func__, nalSize);

        /* detect SPS/PPS/Frame data */
        if(nalSize > (int)size)
        {
            WRAPPER_ERROR("\n%s=> Wrong input stream nal_size=%d, size=%d", __func__, nalSize, size);
            err = EINVAL;
            break;
        }

        if(sps_pps_found)
        {
            if(nalSize + encH->config_size < OMAX_CONFIG_DATA_BUFFER_SIZE)
            {
                memcpy(encH->configPtr + encH->config_size, buf, nalSize);
                encH->config_size += nalSize;
            }
            else
            {
                WRAPPER_ERROR("\n%s=> config data buffer too small(nal_size=%d, size=%d)", __func__, nalSize, size);
                err = ENOMEM;
                break;
            }
        }
        else if(encH->config_size > 0)
        {
            break;
        }

        buf += nalSize;
        size -= nalSize;
    }

    encH->large_input_buf_data_size = size;
    encH->currPtr = buf;

    decoder_info_t decoder_info;
    avc1_decoder_specific_t *avc_info;
    if(h264_parse_decoder_config(encH->configPtr, encH->config_size, &decoder_info, &avc_info) == EOK)
    {
        encH->mLumaDepth = avc_info->bit_depth_luma;
        encH->mChromaDepth = avc_info->bit_depth_chroma;
        encH->mChromaFmt = avc_info->chroma_format_idc;
        h264_free_decoder_specific(avc_info);
    }
    else
    {
        WRAPPER_ERROR("\n%s=> Failed to get config data(%d) ", __func__, encH->config_size);
        err = EINVAL;
    }

    WRAPPER_PRINTF("\n%s get config(%u) input buffer(encH->currPtr=%p, encH->large_input_buf_data_size=%d)", __func__,
            encH->config_size, encH->currPtr, encH->large_input_buf_data_size);

#if defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2) || defined(SOC_AM62A)
    /* Return the read pointer back to beginning of the file */
    lseek(encH->in_fd, 0, SEEK_SET);
#endif /* SOC_J721S2 or SOC_J784S4 or SOC_J742S2 or SOC_AM62A */

    return err;
}

int32_t readFrame(OmxilVideoEncDec_t *encH, OMX_BUFFERHEADERTYPE *bufHdr)
{
#if defined(SOC_J721E)
    static const uint8_t sc[] = {0x00, 0x00, 0x01};  /* start code */
    int initial_sc_offset = 0;
    int sps_pps_found = 0;
    int frame_found = 0;
    int au_delimiter_found = 0;
    int nalSize;
    uint8_t *buf;
    uint32_t      size;
    const uint8_t *inbuf = encH->currPtr;
    int32_t err = EOK;

    WRAPPER_PRINTF("\n%s (currPtr=%p, dataSize=%d)", __func__, encH->currPtr, encH->large_input_buf_data_size);

    if(encH->large_input_buf_data_size < (int32_t)(DIM(sc) + 2))
    {
        if(encH->eof_received == omxil_true_e)
        {
            WRAPPER_PRINTF("\n%s=> reach the end of data", __func__);
            encH->doneReadFrames = omxil_true_e;
            return EOK;
        }
        else
        {
            uint8_t *rbuf = encH->large_input_buf;
            if(encH->large_input_buf_data_size > 0)
            {
                memcpy(rbuf, encH->currPtr, encH->large_input_buf_data_size);
                rbuf += encH->large_input_buf_data_size;
            }
            encH->large_input_buf_data_size = read(encH->in_fd, rbuf, OMAX_DEFAULT_BUFFER_SIZE);
            WRAPPER_PRINTF("\n%s read(inputBufPtr=%p, dataSize=%d)", __func__, encH->large_input_buf, encH->large_input_buf_data_size);
            if(encH->large_input_buf_data_size <= 0) {
                WRAPPER_PRINTF("\n%s=> reach the end of data", __func__);
                encH->eof_received = omxil_true_e;
                return EOK;
            }
        }
    }

    size = encH->large_input_buf_data_size;
    buf = encH->currPtr;
    bufHdr->nFilledLen = 0;

    /* check if the buffer starts with the start code, else skip the bytes */
    initial_sc_offset = FindStartCode(sc, DIM(sc), inbuf, encH->large_input_buf_data_size, NULL, NULL);
    WRAPPER_PRINTF("\n%s=> start code was found at initial_sc_offset %d (skipping these bytes)", __func__, initial_sc_offset);
    if(initial_sc_offset > encH->large_input_buf_data_size)
    {
        WRAPPER_ERROR("\n%s=> Wrong input stream initial_sc_offset=%d, encH->large_input_buf_data_size=%d", __func__, initial_sc_offset, encH->large_input_buf_data_size);
        return EINVAL;
    }
    buf += initial_sc_offset;
    size -= initial_sc_offset;

    WRAPPER_PRINTF("\nbuf=%p, size=%d, (DIM(sc) + 2)=%lu", buf, size, (DIM(sc) + 2));

    if (size < (DIM(sc) + 2))
    {
        WRAPPER_PRINTF("\n%s=> Only frame header found(size=%d), skip", __func__, size);
        return EOK;
    }

    while((size >= DIM(sc)+ 2))
    {
        /* detect SPS/PPS/Frame data */
        int nal_type = H264CheckNalType((buf + DIM(sc)), &sps_pps_found, &frame_found);

        if(nal_type == NAL_AUD)
        {
            /* return access unit */
            if(au_delimiter_found)
            {
                break;
            }
            else
            {
                au_delimiter_found = 1;
            }
        }

        /* if we've already started pushing NALU to the buffer, check
         * if this next NALU starts a new access unit
         */
        nalSize = DIM(sc) + FindStartCode(sc, DIM(sc), buf+DIM(sc), size-DIM(sc), NULL, NULL);

        if(nalSize > (int)size)
        {
            WRAPPER_ERROR("\n%s=> Wrong input stream nal_size=%d, size=%d", __func__, nalSize, size);
            err = EINVAL;
            break;
        }

        memcpy(bufHdr->pBuffer + bufHdr->nFilledLen, buf, nalSize);
        buf += nalSize;
        size -= nalSize;
        bufHdr->nFilledLen += nalSize;

        /* no access unit delimiter return once frame data found */
        if(!au_delimiter_found && frame_found)
        {
            break;
        }
    }
    if(size == 0 && encH->eof_received == omxil_false_e)
    {
        encH->large_input_buf_data_size = read(encH->in_fd, encH->large_input_buf, OMAX_DEFAULT_BUFFER_SIZE);
        if(encH->large_input_buf_data_size > 0)
        {
            WRAPPER_PRINTF("\n%s:%d=> read more data=%d", __func__, __LINE__, encH->large_input_buf_data_size);
            buf = encH->large_input_buf;
            size = encH->large_input_buf_data_size;
            nalSize = FindStartCode(sc, DIM(sc), buf, size, NULL, NULL);
            if(nalSize > (int)size)
            {
                WRAPPER_ERROR("\n%s:%d=> Wrong input stream nal_size=%d, size=%d", __func__, __LINE__, nalSize, size);
                err = EINVAL;
            }
            else
            {
                memcpy(bufHdr->pBuffer + bufHdr->nFilledLen, buf, nalSize);
                bufHdr->nFilledLen += nalSize;
                buf += nalSize;
                size -= nalSize;
            }
            if(encH->large_input_buf_data_size < OMAX_DEFAULT_BUFFER_SIZE)
            {
                WRAPPER_PRINTF("\n%s:%d=> reach end of file %d", __func__, __LINE__, encH->large_input_buf_data_size);
                encH->eof_received = omxil_true_e;
            }
        }
        else
        {
            WRAPPER_ERROR("\n%s=> failed to read data=%d", __func__, encH->large_input_buf_data_size);
            err = EINVAL;
        }
    }

    encH->large_input_buf_data_size = size;
    encH->currPtr = buf;

    if(frame_found == 0)
    {
        bufHdr->nFilledLen = 0;
    }

    WRAPPER_PRINTF("\n%s get one frame(%u) input buffer(bufHdr=%p, bufPtr=%p, encH->large_input_buf_data_size=%d)", __func__,
            bufHdr->nFilledLen, bufHdr, bufHdr->pBuffer, encH->large_input_buf_data_size);

    return err;
#else
    bufHdr->nFilledLen = read(encH->in_fd, bufHdr->pBuffer, encH->inputPortBufSize);
    if(bufHdr->nFilledLen <= 0)
    {
        WRAPPER_PRINTF("\n%s:%d=> reach end of file %d", __func__, __LINE__, bufHdr->nFilledLen);
        encH->eof_received = omxil_true_e;
        encH->doneReadFrames = omxil_true_e;
    }

    return EOK;
#endif /* SOC_J721E */
}
