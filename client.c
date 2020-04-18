#include<stdio.h>
#include<stdlib.h>
#include<netdb.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<time.h>
#include<string.h>


int main(int argc, char *argv[]){
	char message[100];
	int server, portNumber;
	socklen_t len;
	struct sockaddr_in servAdd;
	int dice;
	int32_t point;
	int size=sizeof(point);
	long int ss=0;
	if(argc != 3){
		printf("Call model:%s <IP> <Port#>\n",argv[0]);
		exit(0);
	}
	if ((server=socket(AF_INET,SOCK_STREAM,0))<0){
		fprintf(stderr, "[-] Cannot create socket\n");
		exit(1);
	}
	else
	{
		fprintf(stderr, "[+] Socket Created\n");
	}
	
	servAdd.sin_family = AF_INET;
	sscanf(argv[2], "%d", &portNumber);
	servAdd.sin_port = htons((uint16_t)portNumber);
	if(inet_pton(AF_INET, argv[1],
		&servAdd.sin_addr) < 0){
		fprintf(stderr, " inet_pton() has failed\n");
		exit(2);
	}
	if(connect(server, (struct sockaddr *) &servAdd,
		sizeof(servAdd))<0){
		fprintf(stderr, "connect() failed, exiting\n");
		exit(3);
	}
	while(1){
		if (read(server, message, sizeof(message))<0){ // sizeof(message)
			fprintf(stderr, "read() error\n");
			exit(0);
		}
		printf("%s\n",message);
		if(!strcmp(message,"You can now play")){
			dice = (int) time(&ss)%6+1;
			printf("********** You got: %d **********\n",dice);
			point=htonl(dice);
			write(server,&point,size);
		}
	}
	exit(0);
}
