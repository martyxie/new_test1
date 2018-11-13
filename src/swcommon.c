/**
 * @file swcommon.c
 * @brief 
 * @author YOUR NAME (), 
 * @version 1.0
 * @history
 * 		参见 :
 * 		2012-11-21 YOUR NAME created
 */

#include "swlog.h"
#include "swcommon.h"

static int m_endian_mode = UNKNOW_ENDIAN;

int sw_get_endian_type(void)
{
	int result;
	union tmp_t
	{
		int a;
		char b;
	}tmp;
	tmp.a = 1;
	if(tmp.b == 1)
	{
		SW_LOG_DEBUG("this dev is LITTLE_ENDIAN\n");
		result = LITTLE_ENDIAN;
	}
	else
	{
		SW_LOG_DEBUG("this dev is BIG_ENDIAN\n");
		result = BIG_ENDIAN;
	}
	m_endian_mode = result;
	return result;
}

