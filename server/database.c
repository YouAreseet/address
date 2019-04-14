#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include "../address.h"


char c_database_name[32] = {0};


/**********************
1、创建数据库文件
2、创建保存联系人信息的表
**********************/
void InitDataBase()
{
    sqlite3 *ppdb; //数据库句柄

    int ret = sqlite3_open("address.db", &ppdb);
    if (ret != SQLITE_OK)
    {
        printf("sqlite3_open : %s\n", sqlite3_errmsg(ppdb));

	exit(1);
    }

    char sql[128] = {0}; //保存数据库语句
    sprintf(sql, "create table if not exists address (name text primary key, passwd text);");
    ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
    if (ret != SQLITE_OK)
    {
        printf("sqlite3_exec11: %s\n", sqlite3_errmsg(ppdb));
	exit(1);
    }

    sqlite3_close(ppdb);
}


void InitDataBase_address()
{
	sqlite3 *ppdb; //数据库句柄

    int ret = sqlite3_open("address.db", &ppdb);
    if (ret != SQLITE_OK)
    {
        printf("sqlite3_open : %s\n", sqlite3_errmsg(ppdb));

	exit(1);
    }

    char sql[128] = {0}; //保存数据库语句
    sprintf(sql, "create table if not exists '%s' (name text primary key, tel text);", c_database_name);
    ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
    if (ret != SQLITE_OK)
    {
        printf("sqlite3_exec11: %s\n", sqlite3_errmsg(ppdb));
	exit(1);
    }

    sqlite3_close(ppdb);
}


int i_condition_login = 0;

static int send_login(void *para, int columnCount, char **columnValue, char **columnName) 
{
	i_condition_login = 1;
	
	return 0;
}


void log_in(Chat *c, int fd)
{
	sqlite3 *ppdb;
	int result;
	
	int ret = sqlite3_open("address.db", &ppdb);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_open : %s\n", sqlite3_errmsg(ppdb));
    }
	
	char sql[128] = {0};
	
	sprintf(sql, "select * from address where name = '%s' and passwd = '%s';", c->name, c->passwd);
	ret = sqlite3_exec(ppdb, sql, send_login, NULL, NULL);
	if (ret != SQLITE_OK)
    {
        printf("sqlite3_exec_add : %s\n", sqlite3_errmsg(ppdb));
    }
	
	if(1 == i_condition_login)
	{
		result = LOGSUCCESS;
		ret = send(fd, &result, sizeof(result), 0);
		if(-1 == ret)
		{
			perror("send");
		}
		
		strcpy(c_database_name, c->name);
	}
	
	if(0 == i_condition_login)
	{
		result = WRONG;
		ret = send(fd, &result, sizeof(result), 0);
		if(-1 == ret)
		{
			perror("send");
		}
	}
}




int i_condition_adduser = 0;

static int send_adduser(void *para, int columnCount, char **columnValue, char **columnName)
{
	
	
	i_condition_adduser = 1;
	
	return 0;
}


void add_user(Chat *c, int fd)
{
    sqlite3 *ppdb;
	int result;

    int ret = sqlite3_open("address.db", &ppdb);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_open : %s\n", sqlite3_errmsg(ppdb));
    }

    char sql[128] = {0};
	
	sprintf(sql, "select * from address where name = '%s';", c->name);
	ret = sqlite3_exec(ppdb, sql, send_adduser, NULL, NULL);
	if (ret != SQLITE_OK)
    {
        printf("sqlite3_exec_add : %s\n", sqlite3_errmsg(ppdb));
    }

	if(0 == i_condition_adduser)
	{
		sprintf(sql, "insert into address values ('%s', '%s');", c->name, c->passwd);
		ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
		if (ret != SQLITE_OK)
		{
			printf("sqlite3_exec : %s\n", sqlite3_errmsg(ppdb));
		}
		
		strcpy(c_database_name, c->name);
		
		InitDataBase_address();
		
		result = ADDSUCCESS;
		
		ret = send(fd, &result, sizeof(result), 0);
		if(-1 == ret)
		{
			perror("send");
		}
	}
	else
	{
		result = USEREXIST;
	
		int ret = send(fd, &result, sizeof(result), 0);
		if(-1 == ret)
		{
			perror("send");
		}
	}

    sqlite3_close(ppdb);
}





int sendinfo(void *para, int columnCount, char **columnValue, char **columnName)
{
    int fd = *(int *)para;
    Chat c;
    strcpy(c.name, columnValue[0]);
    strcpy(c.passwd, columnValue[1]);

    int ret = send(fd, &c, sizeof(c), 0);
    if(-1 == ret)
    {
        perror("send");
    }

    return 0;
}


void show_user(int fd)
{
    sqlite3 *ppdb;

    int ret = sqlite3_open("address.db", &ppdb);
    if (ret != SQLITE_OK)
    {
        perror("sqlite3_open");
	exit(1);
    }

    char sql[128] = "select * from address;";

    ret = sqlite3_exec(ppdb, sql, sendinfo, &fd, NULL);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_exec : %s\n", sqlite3_errmsg(ppdb));
	exit(1);
    }

    Chat c;
    strcpy(c.name, "bye");
    strcpy(c.passwd, "bye");

    ret = send(fd, &c, sizeof(c), 0);
    if (-1 == ret)
    {
        perror("send");
    }
	
	sqlite3_close(ppdb);
}


int sendinfo_address(void *para, int columnCount, char **columnValue, char **columnName)
{
    int fd = *(int *)para;
    Chat c;
    strcpy(c.name, columnValue[0]);
    strcpy(c.tel, columnValue[1]);

    int ret = send(fd, &c, sizeof(c), 0);
    if(-1 == ret)
    {
        perror("send");
    }

    return 0;
}


void add_info(Chat *c)
{
    sqlite3 *ppdb;

    int ret = sqlite3_open("address.db", &ppdb);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_open : %s\n", sqlite3_errmsg(ppdb));
    }

    char sql[128] = {0};
    sprintf(sql, "insert into '%s' values ('%s', '%s');", c_database_name, c->name, c->tel);
    ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
    if (ret != SQLITE_OK)
    {
        printf("sqlite3_exec : %s\n", sqlite3_errmsg(ppdb));
    }

    sqlite3_close(ppdb);
}


void del_info(Chat *c)
{
	sqlite3 *ppdb;
	
	int ret = sqlite3_open("address.db", &ppdb);
	if(ret != SQLITE_OK)
	{
		printf("sqlite3_open : %s\n", sqlite3_errmsg(ppdb));
	}
	
	char sql[128] = {0};
	sprintf(sql, "delete from '%s' where name = '%s';", c_database_name, c->name);
	ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
	if (ret != SQLITE_OK)
    {
        printf("sqlite3_exec : %s\n", sqlite3_errmsg(ppdb));
		exit(1);
    }

    sqlite3_close(ppdb);
}


void update_info(Chat *c, Chat *c1)
{
	sqlite3 *ppdb;
	
	int ret = sqlite3_open("address.db", &ppdb);
	if(ret != SQLITE_OK)
	{
		printf("sqlite3_open : %s\n", sqlite3_errmsg(ppdb));
	}
	
	char sql[128] = {0};
	sprintf(sql, "update '%s' set name = '%s', tel = '%s' where name = '%s';", c_database_name, c1->name, c1->tel, c->name);
	ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
	if (ret != SQLITE_OK)
    {
        printf("sqlite3_exec : %s\n", sqlite3_errmsg(ppdb));
    }
	
	sqlite3_close(ppdb);
}


void show_info(int fd)
{
    sqlite3 *ppdb;

    int ret = sqlite3_open("address.db", &ppdb);
    if (ret != SQLITE_OK)
    {
        perror("sqlite3_open");
	exit(1);
    }

    char sql[128] = {0};
	sprintf(sql, "select * from '%s';", c_database_name);

    ret = sqlite3_exec(ppdb, sql, sendinfo_address, &fd, NULL);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_exec : %s\n", sqlite3_errmsg(ppdb));
	exit(1);
    }

    Chat c;
    strcpy(c.name, "bye");
    strcpy(c.tel, "bye");

    ret = send(fd, &c, sizeof(c), 0);
    if (-1 == ret)
    {
        perror("send");
    }
	
	sqlite3_close(ppdb);
}


void search_info(Chat *c, int fd)
{
	sqlite3 *ppdb;
	
	int ret = sqlite3_open("address.db",&ppdb);
	if (ret != SQLITE_OK)
	{
		perror("sqlite3_open");
		exit(1);
	}
	
	char sql[128] = {0};
	sprintf(sql,"select * from '%s' where name = '%s';", c_database_name, c->name);
	ret = sqlite3_exec(ppdb, sql, sendinfo_address, &fd, NULL);
	if(ret !=SQLITE_OK)
	{
		printf("sqlite3_exec : %s\n", sqlite3_errmsg(ppdb));
		exit(1);
	}
	ret = send(fd,&c,sizeof(c), 0);
	if(-1 == ret)
	{
		perror("send");
	}
	
	sqlite3_close(ppdb);
}



