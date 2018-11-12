#ifndef __SW_LOG_H__
#define __SW_LOG_H__

#define PLUG_LOGO_LOG( format, ... )  sw_log_out(__func__, __LINE__, format, ##__VA_ARGS__)

#endif
