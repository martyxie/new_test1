/****
*@file swtcpserver.c 
*@brief 服务器main函数程序
*@author xiexiang
*@history 
*       2016-04-18 xiexiang created
*
***/
#include<stdio.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "swsocket.h"
#include "base64.h"
#include "swjson.h"

#define FILENAMEBUF_SIZE 128
#define CLIENTIP_SIZE 64

typedef struct share_info
{
	int clientfd; //socket通信描述符
	//线程间的互斥锁
	pthread_mutex_t *pshare_lock;

}share_info_t;

static int send_login_server(int fd)
{
	char commandbuf[1024]; //保存传过来的命令，要上传还是下载
	char data[1024] = {0};
	char base_data[1024] = {0};
	int len = 0;
	int now  = time(NULL);
	len = snprintf(data,sizeof(data),"{\"result\":0,\"sequence\":\"12345\",\"sessionKey\":\"1000111122223333\",\"tcpHost\":\"10.10.18.252:8081\",\"udpHost\":\"10.10.18.252:8081\",\"token\":\"10001sunniwell\",\"time\":%d}",now);
	printf("data = %s\n",data);
	//base 64编码
	if(base64encode(data,len,base_data,sizeof(base_data))<0)
		printf("base code fail\n");
	memset(commandbuf,0,sizeof(commandbuf));
	len = snprintf(commandbuf,sizeof(commandbuf),"{\"code\":1003,\"data\":\"%s\"}",base_data);
	printf("send data = %s\n",commandbuf);
	//回应客户端可以开始传送文件了
	if(send(fd,commandbuf,len,0) <= 0 )
	{
		printf("fail to Respond\n");
	}
	return 0;
}
static int send_login_heartbeat(int fd)
{
	char commandbuf[1024] ={0}; //保存传过来的命令，要上传还是下载
	char data[1024] = {0};
	char base_data[1024] = {0};
	int len = 0;
	int now  = time(NULL);
	len = snprintf(data,sizeof(data),"{\"result\":0,\"sequence\":\"12345\",\"heartBeat\":\"100\",\"authInterval\":\"600\",\"time\":%d}",now);
	printf("data = %s\n",data);
	//base 64编码
	if(base64encode(data,len,base_data,sizeof(base_data))<0)
		printf("base code fail\n");
	memset(commandbuf,0,sizeof(commandbuf));
	len = snprintf(commandbuf,sizeof(commandbuf),"{\"code\":1005,\"data\":\"%s\"}",base_data);
	printf("send data = %s\n",commandbuf);
	//回应客户端可以开始传送文件了
	printf("fd = %d,%d\n",fd,__LINE__);
	int ret = send(fd,commandbuf,len,0);
	if( ret <= 0 )
	{
		printf("fail to Respond\n");
	}
	printf("ret = %d\n",ret);
	return 0;
}
static int heart_beat(int fd)
{
	char commandbuf[1024] ={0}; //保存传过来的命令，要上传还是下载
	char data[1024] = {0};
	char base_data[1024] = {0};
	int len = 0;
	int now  = time(NULL);
	len = snprintf(data,sizeof(data),"{\"result\":0,\"sequence\":\"12345\"\"time\":%d}",now);
	printf("data = %s\n",data);
	//base 64编码
	if(base64encode(data,len,base_data,sizeof(base_data))<0)
		printf("base code fail\n");
	memset(commandbuf,0,sizeof(commandbuf));
	len = snprintf(commandbuf,sizeof(commandbuf),"{\"code\":1001,\"data\":\"%s\"}",base_data);
	printf("send data = %s\n",commandbuf);
	//回应客户端可以开始传送文件了
	printf("fd = %d,%d\n",fd,__LINE__);
	int ret = send(fd,commandbuf,len,0);
	if( ret <= 0 )
	{
		printf("fail to Respond\n");
	}
	printf("ret = %d\n",ret);
	return 0;
}

//服务线程函数
void server_thread(void *arg)
{
	share_info_t *share_info = (share_info_t *)arg;
	
	int clientfd = share_info->clientfd;
	printf("fd = %d\n",clientfd);
	//解锁
	pthread_mutex_unlock(share_info->pshare_lock);
	int ret = 0;
	char commandbuf[1024]; //保存传过来的命令，要上传还是下载
	memset(commandbuf, 0, sizeof(commandbuf));
	//保存传过来的要保存文件路径或及文件名
	char filenamebuf[FILENAMEBUF_SIZE]; 
	memset(filenamebuf, 0, sizeof(filenamebuf));
	
	//接收客户端的请求命令
	while(1)
	{
		ret = recv(clientfd, commandbuf, sizeof(commandbuf), 0); 
		if( ret <= 0 )
		{
			printf("fail to recv command\n");
			sw_destroy_socket(clientfd);
			pthread_exit(NULL);
		}

		commandbuf[ret] = '\0';
		printf("command = %s\n",&commandbuf[0]); 
		sw_json_t *json = NULL;
		char json_buf[300] = {0};
		char decode[1024] = {0};
		int code = 0;
		json = sw_json_decode(commandbuf,strlen(commandbuf),json_buf,sizeof(json_buf));
		if(json)
		{
			if(json->type != VTYPE_OBJ)
				printf("not object\n");
			int i = 0;
			for(i=0;i<json->jobj->count;i++)
			{
				printf("json->jobj->nvs[%d].name = %s\n",i,json->jobj->nvs[i].name);
				printf("json->jobj->nvs[%d].value.str = %s\n",i,json->jobj->nvs[i].value.str);
				if(strncmp(json->jobj->nvs[i].name,"code",strlen("code")) == 0)
					code = atoi(json->jobj->nvs[i].value.str);
				if(strncmp(json->jobj->nvs[i].name,"data",strlen("data")) == 0)
				{
					base64decode(json->jobj->nvs[i].value.str,strlen(json->jobj->nvs[i].value.str),decode,sizeof(decode));
					printf("decode = %s\n",decode);
				}
			}
		}
		if(code == 1002)
		{
			send_login_server(clientfd);
			break;
		}
		else if(code == 1004)
			//send_login_server(clientfd);
			send_login_heartbeat(clientfd);
		else if(code == 1000)
			heart_beat(clientfd);

		continue;
	}

	switch(commandbuf[0])
	{
		case 'g': printf("download %s\n",filenamebuf);
				  // 发送文件
			 break;
				  
		case 'p': printf("upload %s\n",filenamebuf);
			//接收文件
			if(send(clientfd,"ok",3,0) <= 0)
				 printf("fail to tell client ok\n");  
			 printf("succes recv\n");
			 break;
		default : printf("nothing to do\n");
				  
							  
	}
	while(1)
	{
		sleep(1);
	}
	printf("destroy client socket\n");
	//关闭通信socket
	sw_destroy_socket(clientfd);
	//退出线程
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	
	struct sockaddr_in clientaddr; //用来保存客户端的ip
	bzero(&clientaddr,sizeof(clientaddr));
	socklen_t addrlen = sizeof(clientaddr);
	
	int listenfd =  sw_create_socket(AF_INET, SOCK_STREAM, 0);
	if( listenfd < 0 )
	{
		printf("fail to creat socket\n");
		return -1;
	}
	printf("listenfd = %d\n",listenfd);
	if( sw_bind_socket(listenfd,"10.10.18.252", 8081) == 0 )
	{
		listen(listenfd,5);
	}
	else
	{
		perror("fail to bind \n");
		sw_destroy_socket(listenfd);
		return -1;
	}
	
	pthread_mutex_t main_lock;
	pthread_mutex_init(&main_lock, NULL);
	
	share_info_t share_info;
	share_info.clientfd = -1; //新连接上来的客户端
	share_info.pshare_lock = &main_lock;
	//上锁
	pthread_mutex_lock(&main_lock);
	
	char clientip[CLIENTIP_SIZE];
	memset(clientip, 0,sizeof(clientip));
	pthread_t server_thread_tid = 0;
	while( (share_info.clientfd = accept(listenfd, 
			(struct sockaddr *)&clientaddr,&addrlen)) > 0 )
	{
		pthread_mutex_unlock(&main_lock);
		//打印客户端的ip
		if( inet_ntop(AF_INET,(void *)&clientaddr.sin_addr,
		    clientip,20) != NULL ) 
		{
			printf("%s is connect\n", clientip);
		}
		printf("begin \n");
		pthread_mutex_lock(&main_lock);
		//创建线程
		if( pthread_create( &server_thread_tid, NULL,
                          (void *)server_thread,(void *)&share_info) < 0 )
		{
			printf("fail to creat thread\n");
			//pthread_mutex_unlock(&main_lock);
			sw_destroy_socket(share_info.clientfd);
			continue;  
		}
		 
		pthread_mutex_lock(&main_lock);
		//分离线程 线程结束后自动回收资源
		if( pthread_detach(server_thread_tid) < 0 )
			printf("fail to detach thread\n");
	
	}
	pthread_mutex_destroy(&main_lock);
	sw_destroy_socket(listenfd);
	return 0;

} 
