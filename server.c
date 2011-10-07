/*server.c reads characters from the client and returns the uppercase to the server.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLINE 80
#define SERV_PORT 8000

int main(){
    struct sockaddr_in servaddr, cliaddr;
    socklen_t cliaddr_len;
    int listenfd, connfd;
    char buf[MAXLINE];
    char str[INET_ADDRSTRLEN];
    int i, n;
  
    /*分配一个网络通讯接口*/
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    
    /*将结构体清零*/
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
    
    /*在服务端绑定一个接口*/
    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    /*声明listenfd处于监听状态，并且最多允许有20个客户端处于连接状态*/
    listen(listenfd, 20);
    
    printf("Accepting connection...\n");
    
    while(1){
	cliaddr_len = sizeof(cliaddr);
        /*服务器接收连接，accept()返回传出客户端的地址和端口号*/
	connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
	while(1){
	    n = read(connfd, buf, MAXLINE);
	    if (n == 0){
		printf("The other side has been closed.\n");
                break;
	    }
	    printf("received from %s at PORT %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)), ntohs(cliaddr.sin_port));
	    for (i = 0; i < n; i++)
		buf[i] = toupper(buf[i]);
	    write(connfd, buf, n);
	}
	close(connfd);
    }
}
