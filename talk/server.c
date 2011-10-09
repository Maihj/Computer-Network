/*Receive the client's request, and than print what the client has said.*/ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUF_SIZE 1024
#define SERVER_PORT 8000
#define BACKLOG 1

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

int Bind(int sockfd, const struct sockaddr *myaddr, socklen_t addrlen){
    int n;
    if ((n = bind(sockfd, myaddr, addrlen)) < 0){
	printf("bind error\n");
	exit(1);
    }
    printf("Bind is OK.\n");
    return n;
}

int Listen(int sockfd, int backlog){
    int n;
    if ((n = listen(sockfd, backlog)) < 0){
	printf("listen error\n");
	exit(1);
    }
    printf("Listen client...\n");
    return n;
}

void DoClientRequest(int clientSocket){
    char buf[BUF_SIZE];
    int n;
    
    /*接收客户端发送的请求*/
    while(1){
	n = recv(clientSocket, buf, BUF_SIZE, 0);
	if (n < 0){
	    printf("%d > client error\n", clientSocket);
	    return;
	}
	if (n == 0){
	    printf("%d > client exit\n", clientSocket);
	    return;
	}

	/*解析接收数据内容并显示*/
	printf("%d ===> %s\n", clientSocket, buf);
    }
}

int main(int argc, char* argv[]){
    struct sockaddr_in servaddr, cliaddr;   //sockaddr_in数据结构
    int serverSocket, clientSocket;         //定义两个文件描述符
    socklen_t cliaddr_len;
    int n;
    
    /*分配一个网络通讯接口，IPv4地址，TCP协议*/
    serverSocket = Socket(AF_INET, SOCK_STREAM, 0);
    
    /*清空结构体servaddr，并设置地址类型、端口号*/
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //将本地IP转化为网络协议中规定的格式，IP地址任意
    servaddr.sin_port = htons(SERVER_PORT);
    
    /*绑定一个服务器地址端口*/
    Bind(serverSocket, (struct sockaddr *) &servaddr, sizeof(servaddr));
    
    /*使serverSocket处于监听状态，且只允许一个客户端连接*/
    Listen(serverSocket, BACKLOG);

    /*服务器接收连接，accept()返回传出客户端的地址和端口号*/
    cliaddr_len = sizeof(cliaddr);
    clientSocket = accept(serverSocket, (struct sockaddr *) &cliaddr, &cliaddr_len);
    if (clientSocket < 0){
	printf("accept error\n");
	exit(1);
    }
    printf("Client accepted, socket: %d\n", clientSocket);

    /*接收和处理从客户端发送的请求*/
    DoClientRequest(clientSocket);
	
    /*关闭客户端的文件描述符clientSocket*/
    close(clientSocket);
    return 0;
}
