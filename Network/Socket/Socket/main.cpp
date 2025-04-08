
#pragma region 府椽胶 扁馆 家南 积己
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <unistd.h>
//#include<arpa/inet.h>
//#include<sys/types.h>
//#include <sys/socket.h>
//
//void error_handling(char* message);
//int main(int argc, char** argv) {
//	int serv_sock;
//	int clnt_sock;
//	struct sockaddr_in serv_addr;
//	struct sockaddr_in clnt_addr;
//
//	int clnt_addr_size;
//	char message[] = "HELLO WORLD!";
//
//	if (argc != 2) {
//		printf("Usage : %s <port>\n", argv[0]);
//	}
//
//	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
//	if (serv_sock == -1)
//		error_handling("socket() error");
//	memset(&serv_addr, 0, sizeof(serv_addr));
//	serv_addr.sin_family = AF_INET;
//	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
//	serv_addr.sin_port = htons(atoi(argv[1]);
//
//	if (bind(serv_sock, (strtuct sockaddr*) & serv_addr, sizeof(serv_addr)) == -1)
//		error_handling("bind() error");
//
//	if (listen(serv_sock, 5) == -1)
//		error_handling("Listen() error");
//
//	clnt_addr_size = sizeof(clnt_addr);
//
//	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt & addr, &clnt_addr_size);
//	if (clnt_sock == -1)
//		error_handling("accept() error");
//
//	write(clnt_sock, message, sizeof(message));
//	fputs(message, strerr);
//	fputc('\n', stderr);
//	close(clnt_sock);
//	return 0;
//}
#pragma endregion

#pragma region 扩档快 扁馆 家南 积己
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

void ERRORHandling(const char* message);

int main(int argc, char** argv) {
    WSADATA wsaData;

    short hostOrdPort = 0x1234;
    short netOrdPort;

    long hostOrdAdd = 0x12345678;
    long netOrdAdd;

    
    //if (argc != 2) {
    //    printf("Usage: %s <port>\n", argv[0]);
    //    exit(1);
    //}

    /* WIN SOCK 檬扁拳 */
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ERRORHandling("WSAStartup() error!");
     
    netOrdPort = htons(hostOrdPort);
    netOrdAdd = htonl(hostOrdAdd);

    printf("HostOrdered Port : %x\n", hostOrdPort);

    WSACleanup();
    return 0;
}

void ERRORHandling(const char* message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

#pragma endregion