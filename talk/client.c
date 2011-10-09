#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUF_SIZE 1024
#define SERVER_PORT 8000

/*封装函数，用以处理异常*/
int Socket(int family, int type, int protocol){
    int n;
    if ((n = socket(family, type, protocol)) < 0){
	printf("server socket error\n");
	exit(1);
    }
    printf("Server socket is OK.\n");
    return n;
}

int Connect(int sockfd, const struct sockaddr *servaddr, socklen_t addrlen){
    int n;
    if ((n = connect(sockfd, servaddr, addrlen)) < 0){
	printf("connect error\n");
	exit(1);
    }
    printf("Connect is OK\n");
    return n;
}

void DoRequest(int clientSocket){
    char buf[BUF_SIZE];
    int n;
    
    /*接收客户端发送的请求*/
    while(1){
        scanf("%s", buf);
	if (send(clientSocket, buf, sizeof(buf), 0) < 0){
	    printf("send error\n");
	    return;
	}
	printf("Send is OK\n");
    }
}

int main(int argc, char *argv[]){
    int clientSocket, n;
    struct sockaddr_in servaddr;
    
    /*传入连接服务器的参数，要求在终端输入的格式为"./client serverIP"*/
    if (argc != 2){
	printf("usage: client serverIP address\n");
	exit(1);
    }

    clientSocket = Socket(AF_INET, SOCK_STREAM, 0);
    
    /*连接到服务器*/
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    if (inet_aton(argv[1], &servaddr.sin_addr) == 0){
	printf("inet_aton error\n");
	exit(1);
    }
    Connect(clientSocket, (struct sockaddr *) &servaddr, sizeof(servaddr));

    /*发送请求，与服务器通信*/
    DoRequest(clientSocket);
    
    /*关闭文件描述符clientsocket*/
    close(clientSocket);
    printf("Socket close\n");
}
