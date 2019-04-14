#ifndef ADDRESSS_H
#define ADDRESSS_H

#define ADDINFO		1000
#define DELINFO		1001
#define UPDATEINFO	1002
#define	SEARCHINFO	1003
#define SHOWINFO	1004
#define EXIT		1005
#define LOGIN		1006
#define REGINFO		1010
#define SHOWUSER	1011
#define ADDUSER		1012
#define USEREXIST	1013
#define ADDSUCCESS	1014
#define WRONG		1015
#define LOGSUCCESS	1016
#define EXITADDRESS	1017

struct ChatInfo
{
    int cmd;
    char name[32];
    char passwd[32];
	char tel[32];
    int result;//用于保存服务器返回的结果
};
typedef struct ChatInfo Chat;

#endif
