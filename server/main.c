#include <stdio.h>

int main()
{
    int sockfd;

    sockfd = InitNet();

    InitDataBase();// 初始化数据库

    main_handler(sockfd);

    return 0;
}
