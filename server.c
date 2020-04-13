#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#define PORT 4441

// void child(int, char *);

int getRandomInteger(){
	long int ss=0;
    return (int) time(&ss)%6 + 1;
		
}

void servicePlayers(int, char *);

int main(){

	int sockfd, ret;
	struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-] Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	
	if(ret < 0)	{
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", 4444);

	if(listen(sockfd, 10) == 0){
		printf("[+]Listening...\n");
	}else{
		printf("[-]Error in binding.\n");
	}


	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n",inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port) );
		printf("%d\n", newSocket);
		
		if((childpid = fork()) == 0){
			close(sockfd);
			servicePlayers(newSocket, buffer);
			// while(1){
			// 	recv(newSocket, buffer, 1024, 0);
			// 	if(strcmp(buffer, ":exit") == 0){
			// 		printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
			// 		break;
			// 	}else{
			// 		printf("PLAYER NAME: %s\n", buffer);
			// 		send(newSocket, buffer, strlen(buffer),0);
			// 		bzero(buffer, sizeof(buffer));
			// 	}
			}
		}
	close(newSocket);
	return 0;
}

void servicePlayers(int newSocket, char* buffer){
	int client1Score = 0;
	int client2Score = 0;
	int flag = 0;
	int newSocketClient1 = newSocket;
	long int ss=0;
	// if(newSocket - newSocketClient1  == 1){
		while(1){
			if (client1Score >= 100 ){
				flag = 1;
				break;
			}else if (client2Score >= 100){
				flag = 2;
				break;
			}else{
				client1Score += (int) time(&ss)%6 + 1;
				client2Score += (int) time(&ss)%6 + 1;
			}
			send(newSocket, "YOUR DICE SCORE IS :: ", 1024, 0);
			if(newSocket%2 == 0){
				printf("%d \n", client1Score);
				send(newSocketClient1, client1Score, 1024, 0);
			}else{
				printf("%d \n", client2Score);
				send(newSocket, client2Score, 1024, 0);
			}
		// }
		if(flag == 1)
			send(newSocket, "CLIENT 1 WINS", 1024, 0);
		else
			send(newSocket, "CLIENT 2 WINS", 1024, 0);
	// }else{
	// 	printf("WAITING HAJU NATHI AAYO\n");
	// }
}
}




