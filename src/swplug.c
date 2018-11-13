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

int sw_plua_init(int argc,char *argv[])
{
	return 0;
}

int main(int argc,char *argv[])
{
	sw_log_init(LOG_LEVEL_ALL);
	SW_LOG_DEBUG("wellcome to use suuniwell sdk\n");
	return sw_plua_init(argc,argv);
}
