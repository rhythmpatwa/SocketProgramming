#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 


int main(int argc, char*argv[]){

	int socketfd, n = 0;
	struct sockaddr_in server;
	char *end = "exit";
	char *next = "history";
	char *hlp = "help";
	char *commands = "1) fibonacci x\n-Enter valid integer in place of 'x' and output is the xth number in the Fibonacci series.\n\n 2) sort x y z ....\n-Enter a maximum of 50 integers to sort and output is reverse sorted.\n\n3) random x y\n-Enter valid integer values for 'x' and 'y' and output is a random number between x and y.\n\n4) history\n-Shows the history.\n\n5) exit\n-Close the window.\n";
	char recvBuff[2000];
	char send[100];
	char c;
	
	if((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("\nSocket creation unsuccessful!\n");
		return 1;
	}
	
	server.sin_family = AF_INET;
      	server.sin_addr.s_addr = inet_addr("127.0.0.1");
      	server.sin_port = htons(7023);
      	
      	if(connect(socketfd, (struct sockaddr *)&server, sizeof(server)) < 0){
       		printf("\nConnection failed: Retry!\n");
       		return 1;
   	}
   	
   	while(1){
   		
	   	puts("\nEnter command: ");
	   	gets(send);
	   	int size = strlen(send);
	   	
	   	//prints the history
	   	if(!(strcmp(send, next))){
	   		FILE *fr;
	   		fr = fopen("history.txt", "r");
	   		printf("\nHistory: ");
	   		c = fgetc(fr);
    			while (c != EOF)
    			{	
    				
        			printf ("%c", c);
        			c = fgetc(fr);
   			}
   			fclose(fr);
	   	}
	   	
	   	//prints the help list commands
	   	if(!(strcmp(send, hlp))){
	   		puts(commands);
	   	}
	   	
	   	write(socketfd, send, strlen(send));
	   	
	   	//exits the client thread
	   	if(!(strcmp(send, end)))
	   		break;
		
		//receives the response from the server
	   	if( (n = read(socketfd, recvBuff, sizeof(recvBuff)-1)) > 0){
			recvBuff[n] = 0;
			if(fputs(recvBuff, stdout) == EOF)
			{
		    		printf("\n Error : Fputs error\n");
			}
	    	}
    	}
    	close(socketfd);
    	return 0;
}

