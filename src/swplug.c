/**
 * @file sw_plug.c
 * @brief 
 * @author YOUR NAME (), 
 * @version 1.0
 * @history
 * 		参见 :
 * 		2012-11-21 YOUR NAME created
 */

#include <stdio.h>
#include "swlog.h"
#include "swcommon.h"

int sw_plua_init(int argc,char *argv[])
{
	sw_get_endian_type();
	sw_common_init(BIG_ENDIAN);
	int a = 0x12345678;
	SW_LOG_DEBUG("a= %x\n",a);
	a = sw_swapint32_value(a);
	SW_LOG_DEBUG("a= %x\n",a);
	a = sw_swapint32_value(a);
	SW_LOG_DEBUG("a= %x\n",a);
	return 0;
}

int main(int argc,char *argv[])
{
	sw_log_init(LOG_LEVEL_ALL);
	SW_LOG_DEBUG("wellcome to use suuniwell sdk\n");
	return sw_plua_init(argc,argv);
}
