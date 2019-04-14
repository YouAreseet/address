#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include "../address.h"


int fd[1024] = {0};
pthread_t tid;
extern int i_condition_login;



int InitNet()
{
    struct sockaddr_in server_addr;//服务器信息
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);

    if (-1 == sockfd)
    {
        perror("sockfd");
	    exit(1);
    }

    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&server_addr, 0 , sizeof(server_addr));
    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(2222);
    server_addr.sin_addr.s_addr = inet_addr("192.168.1.111");//nanopi的IP地址
	
	int ret = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	
	if(-1 == ret)
    {
        perror("bind");
	    exit(1);
    }
    
	ret = listen(sockfd, 10);
	if(-1 == ret)
	{
		perror("listen");
		exit(1);
	}
	return sockfd;
}


void exit_client(int fd)
{
    printf("客户端 %d 下线\n", fd);
    close(fd);//关闭TCP连接
    pthread_exit((void *)0);//线程退出
}


void user_handler(int fd)//线程函数
{
    Chat c,c1;
    while(1)
    {
		memset(&c, 0, sizeof(c));
        int ret = recv(fd, &c, sizeof(c), 0);
		if (-1 == ret)
		{
			perror("recv");
		}
		if(c.cmd != 0)
		{
			printf("%d, %s, %s\n", c.cmd, c.name, c.tel);
		}
		switch(c.cmd)
		{
			case ADDINFO:
				add_info(&c);
				break;
			case DELINFO:
				del_info(&c);
				break;
			case UPDATEINFO:
				memset(&c1, 0, sizeof(c1));
				ret = recv(fd, &c1, sizeof(c1), 0);
				if (-1 == ret)
				{
					perror("recv");
				}
				if(c.cmd != 0)
				{
					printf("%d, %s, %s\n", c.cmd, c.name, c.tel);
				}
				
				update_info(&c, &c1);
				break;
			case SEARCHINFO:
				search_info(&c,fd);
				break;
			case SHOWINFO:
				show_info(fd);
				break;
			case EXITADDRESS:
				return;
		}
    }
}


void *client_handler(void *arg)//线程函数
{
    Chat c,c1;
    int fd = *(int *)arg;
    while(1)
    {
		i_condition_login = 0;
		
		memset(&c, 0, sizeof(c));
        int ret = recv(fd, &c, sizeof(c), 0);
		if (-1 == ret)
		{
			perror("recv");
		}
		if(c.cmd != 0)
		{
			printf("%d, %s, %s\n", c.cmd, c.name, c.passwd);
		}
		
		switch(c.cmd)
		{
			case ADDUSER:
				add_user(&c, fd);
				break;
			case SHOWUSER:
				show_user(fd);
				break;
			case EXIT:
				exit_client(fd);
				break;
			case LOGIN:
				log_in(&c, fd);
				if(1 == i_condition_login)
				{
					printf("登陆成功!\n");
					user_handler(fd);
				}
				break;
			/*
			case DELINFO:
				del_info(&c);
				break;
			case SEARCHINFO:
				search_info(&c,fd);
				break;
			case UPDATEINFO:
				memset(&c1, 0, sizeof(c1));
				ret = recv(fd, &c1, sizeof(c1), 0);
				if (-1 == ret)
				{
					perror("recv");
				}
				update_info(&c, &c1);
				break;
			*/
		}
    }
}


void main_handler(int sockfd)
{
    struct sockaddr_in client_addr;//保存客户端信息
    int i = 0;
    int length = sizeof(client_addr);
    while(1)
    {
        for(i = 0; i < 1024; i++)
	{
	    if (0 == fd[i])
	    {
	        break;
	    }
	}
	fd[i] = accept(sockfd, (struct sockaddr *)&client_addr, &length);//接受客户端的连接

	printf("接受客户端连接 %d\n", fd[i]);

	int ret = pthread_create(&tid, NULL, client_handler, &fd[i]);// 启动线程处理连接（一个线程对应一个客户端）
	if (-1 == ret)
	{
	    perror("pthread_create");
	    exit(1);
	}
    }
}
