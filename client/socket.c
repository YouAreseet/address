#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "../address.h"


int InitNet()
{
    struct sockaddr_in server_addr;//服务器信息
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);

    if (-1 == sockfd)
    {
        perror("sockfd");
		exit(1);
    }

    memset(&server_addr, 0 , sizeof(server_addr));
    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(2222);
    server_addr.sin_addr.s_addr = inet_addr("192.168.1.111");//nanopi的IP地址
	char *ptr = "bye";
    int ret = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    
    if(-1 == ret)
    {
        perror("connect");
	exit(1);
    }
    return sockfd;
}

/**************************************************
1、通知服务器，该客户端下线
2、关闭TCP连接
3、退出程序
**************************************************/
void exit_client(int fd)
{
    Chat c;
    c.cmd = EXIT;//表示下线
    int ret = send(fd, &c, sizeof(c), 0);
    
    if (-1 == ret)
    {
        perror("send");
    }

    close(fd);

    exit(0);
}


/***********************************
登陆账户
************************************/
void log_in(int fd)
{
	Chat c;
	int result;
	extern int i_condition_log;
	
	printf("请输入用户名：\n");
	scanf("%s", c.name);
	
	printf("请输入密码：\n");
	scanf("%s", c.passwd);
	
	c.cmd = LOGIN;
	
	int ret = send(fd, &c, sizeof(c), 0);
	if(-1 == ret)
    {
        perror("send");
    }
	
	ret = recv(fd, &result, sizeof(result), 0);
	if (-1 == ret)
	{
		perror("recv");
	}
	
	if(result == LOGSUCCESS)
	{
		system("clear");
		printf("\t\t登陆成功！\n");
		sleep(2);
		i_condition_log = 1;
	}
	
	if(result == WRONG)
	{
		system("clear");
		printf("\t\t密码错误！\n");
		sleep(2);
	}
}


/*添加信息*/
void add_user(int fd)
{
    Chat c;
	int result;

    printf("请输入用户名：\n");
    scanf("%s", c.name);
	
	printf("请输入密码：\n");
	scanf("%s", c.passwd);

    c.cmd = ADDUSER;
    int ret = send(fd, &c, sizeof(c), 0);
    if(-1 == ret)
    {
        perror("send");
    }
	
	ret = recv(fd, &result, sizeof(result), 0);
	if (-1 == ret)
	{
		perror("recv");
	}
	
	if(result == USEREXIST)
	{
		printf("账号已存在!\n");
	}
	
	if(result == ADDSUCCESS)
	{
		printf("信息录入成功！\n");
	}
	
	sleep(1);
}


/*显示信息*/
void show_user(int fd)
{
    Chat c;

    c.cmd = SHOWUSER;
    int ret = send(fd, &c, sizeof(c), 0);
    if(-1 == ret)
    {
        perror("send");
    }

    while(1)
    {
        memset(&c, 0, sizeof(c));
        ret = recv(fd, &c, sizeof(c), 0);
		if (-1 == ret)
		{
			perror("recv");
		}

		if (!strcmp(c.name, "bye") && !strcmp(c.passwd, "bye"))
		{
			break;
		}

		//printf("\t\t用户名 %s\n", c.name);
		printf("\t\t用户名 %s   密码 %s\n", c.name, c.passwd);
    }
    sleep(3);
}


void exit_address(int fd)
{
	Chat c;
	
	c.cmd = EXITADDRESS;
	
	int ret = send(fd, &c, sizeof(c), 0);
	if(-1 == ret)
    {
        perror("send");
    }

}












/*添加信息*/
void add_info(int fd)
{
    Chat c;

    printf("请输入姓名和电话...\n");
    scanf("%s%s", c.name, c.tel);

    c.cmd = ADDINFO;
    int ret = send(fd, &c, sizeof(c), 0);

    if(-1 == ret)
    {
        perror("send");
    }

    printf("信息录入成功！\n");
}


void show_info(int fd)
{
    Chat c;

    c.cmd = SHOWINFO;
    int ret = send(fd, &c, sizeof(c), 0);
    if(-1 == ret)
    {
        perror("send");
    }

    while(1)
    {
        memset(&c, 0, sizeof(c));
        ret = recv(fd, &c, sizeof(c), 0);
		if (-1 == ret)
		{
			perror("recv");
		}

		if (!strcmp(c.name, "bye") && !strcmp(c.tel, "bye"))
		{
			break;
		}

		printf("\t\t姓名 %s     电话 %s\n", c.name, c.tel);
    }
    sleep(3);
}


void search_info(int fd)
{
	Chat c;
	
	printf("请输入您要查找的联系人姓名：\n");
	scanf("%s", c.name);
	
	c.cmd = SEARCHINFO;
	
	int ret = send(fd, &c, sizeof(c), 0);
	if(-1 == ret)
    {
        perror("send");
    }
	
	memset(&c, 0, sizeof(c));
    ret = recv(fd, &c, sizeof(c), 0);
	
	if(!strcmp(c.name, ""))
	{
		printf("未找到指定联系人！\n");
	}
	else
	{
		printf("\t\t姓名 %s     电话 %s\n", c.name, c.tel);
	}
	sleep(3);
}


void del_info(int fd)
{
	Chat c;
	
	printf("请输入要删除的姓名\n");
	scanf("%s", c.name);
	
	c.cmd = DELINFO;
	int ret = send(fd, &c, sizeof(c), 0);
	if (-1 == ret)
	{
		perror("send");
	}
	
	printf("信息删除成功！\n");
}


void update_info(int fd)
{
	Chat c,c1;
	
	printf("请输入要修改联系人的姓名\n");
	scanf("%s", c.name);
	c.cmd = UPDATEINFO;
	
	printf("请输入要修改的联系人的姓名和电话\n");
	scanf("%s%s", c1.name, c1.tel);
	c1.cmd = UPDATEINFO;
	
	int ret = send(fd, &c, sizeof(c), 0);
	if (-1 == ret)
	{
		perror("send");
	}
	
	ret = send(fd, &c1, sizeof(c1), 0);
	if (-1 == ret)
	{
		perror("send");
	}
	
	printf("信息修改成功！\n");
	sleep(1);
}
