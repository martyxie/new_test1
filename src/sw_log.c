/**
 * @file sw_log.c
 * @brief 
 * @author YOUR NAME (), 
 * @version 1.0
 * @history
 * 		参见 :
 * 		2012-11-21 YOUR NAME created
 */


void sw_log_out( const char *fuc,int line,const char* format, ... )
{
#ifdef DEBUG
	va_list args;
	char log_buf[1024*128] = {0};
	//填充日志
	va_start( args, format );
	vsnprintf(log_buf,sizeof(log_buf),format,args);
	va_end( args );
	fprintf(stdout,"[%s,%d]%s",fuc,line,log_buf);
#endif
	return;
}
