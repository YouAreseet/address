#include <stdio.h>

int main()
{
    int sockfd;

    sockfd = InitNet();

    printf("连接服务器成功！\n");

    main_handler(sockfd);//主程序

    return 0;
}
