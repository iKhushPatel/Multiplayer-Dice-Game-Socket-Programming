#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#define PORT 4444

int getRandomInteger(){
	long int ss=0;
    return (int) time(&ss)%6 + 1;
		
}

void servicePlayers(int, int);

int main(){

	int sockfd, ret;
	struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;
	int FLAG_CLIENT_1 = 0;
	int FLAG_CLIENT_2 = 0;
	int client1;
	int client2;
	
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
		if(newSocket %2 ==0)
		{
			FLAG_CLIENT_1 = 1;
			client1 = newSocket;
			printf("CLIENT 1 AAYO");
		}
		else{
			FLAG_CLIENT_2 = 1;
			client2 = newSocket;
			printf("CLIENT 2 AAYO");
		}
		if(FLAG_CLIENT_1 == 1 && FLAG_CLIENT_2 ==1){
			printf("BOTH CLIETN AAVI GAYAYAYYA");
			//if((childpid = fork()) == 0){
				//close(sockfd);
				servicePlayers(client1, client2);
			//}
		}
		printf("Connection accepted from %s:%d\n",inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port) );
		printf("%d\n", newSocket);
		
	}
	close(newSocket);
	return 0;
}

void servicePlayers(int client1, int client2){
	int client1Score = 0;
	int client2Score = 0;
	int flag = 0;
	
	long int ss=0;
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
		//send(client1, "YOUR DICE SCORE IS :: ", 1024, 0);
		//send(client2, "YOUR DICE SCORE IS :: ", 1024, 0);
		sprintf("%s", htonl(client1Score));
		sprintf("%s", htonl(client2Score));
		send(client1, htonl(client1Score), 1024, 0);
		send(client2, htonl(client2Score), 1024, 0);
		if(flag == 1)
			send(client1, "CLIENT 1 WINS", 1024, 0);
		else if(flag == 2)
			send(client2, "CLIENT 2 WINS", 1024, 0);

}
}




