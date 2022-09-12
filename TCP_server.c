/*
tcp server
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    struct sockaddr_in serveraAddr , clientAddr;
    socklen_t clientAddrLen;
    int nFd = 0,linkFd = 0;
    int nRet = 0;
    int nReadLen = 0;
    char szBuff[BUFSIZ] = {0};


    nFd = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == nFd)
    {
        perror("socket:");
        return -1;
    }

    /* socket address */
    memset(&serveraAddr,0,sizeof(struct sockaddr_in));
    serveraAddr.sin_family = AF_INET; //以ipv4协议进行连接
    serveraAddr.sin_addr.s_addr = htonl(INADDR_ANY); //接收所有客户端ip的连接
    serveraAddr.sin_port = htons(8080);//接收8080端口发来的连接


    int isReuse = 1;
    setsockopt(nFd,SOL_SOCKET,SO_REUSEADDR,(const char*)&isReuse,sizeof(isReuse));

    /* connect bind */
    nRet = bind(nFd,(struct sockaddr*)&serveraAddr,sizeof(serveraAddr));
    if(-1 == nRet)
    {
        perror("bind:");
        return -1;
    }

   
    listen(nFd,1);
    clientAddrLen = sizeof(struct sockaddr_in);
    linkFd = accept(nFd,(struct sockaddr*)&clientAddr,&clientAddrLen);
    if(-1 == linkFd)
    {
        perror("accept:");
        return -1;
    }

    /* port */
    printf("commect %s %d successful\n",inet_ntoa(clientAddr.sin_addr),ntohs(clientAddr.sin_port));

    /* cycle read */
    while(1)
    {
        memset(szBuff,0,BUFSIZ);
        nReadLen = read(linkFd,szBuff,BUFSIZ);
        if(nReadLen > 0)
        {
            printf("read data: %s\n",szBuff);
        }
    }

    return 0;
}
