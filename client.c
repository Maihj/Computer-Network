/*client.c received a string and send to a server, and then get the string from the server and print.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLINE 80
#define SERV_PORT 8000

int main(int argc, char *argv[]){
    struct sockaddr_in servaddr;
    char buf[MAXLINE];
    int sockfd, n;
    char *str;
    
    /*if (argc != 2){
	fputs("usage: ./client message\n", stderr);
	exit(1);
    }
    str = argv[1];*/
    
    /*分配一个文件描述符sockfd，打开一个网络通讯接口，IPv4地址，TCP协议，表示面向流的传输协议*/
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    /*将结构体servaddr(服务端)清空，并设置地址类型，端口号*/
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr); /*字符串转IP地址*/
    servaddr.sin_port = htons(SERV_PORT);
    
    /*向服务端提出连接请求*/
    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    //while(1);
    while (fgets(buf, MAXLINE, stdin) != NULL){
	write(sockfd, buf, strlen(buf));
	n = read(sockfd, buf, MAXLINE);
	if (n == 0)
	    printf("The other side has been closed.\n");
	else
	    write(STDOUT_FILENO, buf, n);
    }
    /*成功连接，则向服务器发送字符串*/
 //    write(sockfd, str, strlen(str));

    /*从服务端获得的字符串*/
 /*    n = read(sockfd, buf, MAXLINE);
    printf("Response from server:\n");
    write(STDOUT_FILENO, buf, n);
    printf("\n");
 */    
    /*关闭网络通讯接口*/
    close(sockfd);
    return 0;
}
