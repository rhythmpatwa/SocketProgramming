#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>

int parseData(char *, int *, int *);
int fibonacci(int );
int randomgen(int , int );
void heapify(int *, int , int );
char *heapSort(int *, int );
void *serviceClient(void *);

pthread_mutex_t fibonacci_lock;
pthread_mutex_t parse_lock;
pthread_mutex_t sort_lock;
pthread_mutex_t random_lock;
pthread_mutex_t request_lock;

char help[][20] = { "fibonacci",
		    "sort",
		    "random",
		    "exit",
		    "history",
		    "help"};
  
int main(int argc, char *argv[]){
	int listenfd , connfd, socklen;
	struct sockaddr_in server , client;
	
		
	//create socket of the given domain, type and protocol and check if it was successful
	listenfd = socket(AF_INET , SOCK_STREAM , 0);
	if (listenfd == -1)
	{
		printf("Socket creation unsuccessful!");
	}
	
	//fill in the fields of the server structure.
	server.sin_family = AF_INET;
      	server.sin_addr.s_addr = INADDR_ANY;
      	server.sin_port = htons(7023);
      	
      	//Bind the corresponding listening file descriptor created with the server address
     	if(bind(listenfd,(struct sockaddr *)&server , sizeof(server)) < 0)
     	{
       		puts("bind failed");
       		return 1;
    	}
    	
     	puts("Bind successful");

     	//Start listening for connections on the listenfd
     	listen(listenfd , 10);

      	//Accept an incoming connection
      	puts("Waiting for incoming connections.....");
      	socklen = sizeof(struct sockaddr_in);
      	
        //wait for the client requests and spawn new thread to service the requests
        while((connfd = accept(listenfd, (struct sockaddr *)&client, (socklen_t*)&socklen))){
        	puts("Connection accepted!");
        	puts("Awaiting requests...");
        	pthread_t handler;
        	
        	pthread_mutex_lock(&request_lock);
        	int *connectfd = (int *)malloc(sizeof(listenfd));
        	*connectfd = connfd;
        
        	pthread_create(&handler, NULL, serviceClient, (void *)connectfd);
        	pthread_mutex_unlock(&request_lock);
        }
        
    	close(listenfd);	
    	return 0;
}


//parses the command sent in by the client 
int parseData(char *data, int *input, int *size){
	const char *delim = " ";
	char *token;
	int token1;
	*size = 0;
	token = strtok(data, delim);
	
	if(!(strcmp(token, help[0]))){
		*input = atoi(strtok(NULL, delim));
		return 1;
	}
	
	else if(!(strcmp(token, help[1]))){
		while(token != NULL){
			token = strtok(NULL, delim);
			if(token){
				token1 = atoi(token);
				input[*size] = token1;
				*size = *size + 1;	
			}
		}
		return 2;
	}
	
	else if(!(strcmp(token, help[2]))){
		while(token != NULL){
			token = strtok(NULL, delim);
			if(token){
				token1 = atoi(token);
				input[*size] = token1;
				*size = *size + 1;	
			}
		}
		if((*size) != 2)
			return 0;
		return 3;
	}
	
	else if(!(strcmp(token, help[3])))
		return 4;
		
	else if(!(strcmp(token, help[4])))
		return 5;
		
	else if(!(strcmp(token, help[5])))
		return 6;
	
	return 0;
	
}

// returns the (num)th element of the Fibonacci series
int fibonacci(int num)
{
	if (num == 0)
        	return 0;
    
    	else if (num == 1)
        	return 1;
    
    	else
        	return(fibonacci(num - 1) + fibonacci(num - 2));
}

//returns a random number between upper and lower limits 
int randomgen(int lower, int upper){
	int result = (rand() % upper);
	result = (result < lower) ? (result + lower) : (result);
	if(result > upper)
		result = upper;
	return result;
}


// returns the elements in decreasing order 
char *heapSort(int *arr, int n){
	
    	for (int i = ((n-1)/2); i >= 0; i--)
        	heapify(arr, n, i);
 	
    	for (int i=n-1; i>=0; i--)
    	{	
        	int temp = arr[i];	// Move current root to end
        	arr[i] = arr[0];
        	arr[0] = temp;
        	heapify(arr, i, 0);	// call min heapify on the reduced heap
    	}
    	
    	//converting the integer array to string and sending the result
    	char *str = (char *)malloc(1);
    	char *result = (char *)malloc(1);
    	sprintf(str, "%d", arr[0]);
    	strcat(result, str);
    	
    	for(int i = 1; i < n; i++){
        	sprintf(str, " %d", arr[i]);
        	strcat(result, str);
    	}
    	free(str);
    	return result;
}

void heapify(int *arr, int n, int i)
{
	int smallest = i;  
	int l = 2*i;  
	int r = 2*i + 1;  

	// If left child is smaller than root
	if (l < n && arr[l] < arr[smallest])
		smallest = l;

	// If right child is smaller than smallest so far
	if (r < n && arr[r] < arr[smallest])
		smallest = r;

	// If smallest is not root
	if (smallest != i)
	{
		int temp = arr[i];
		arr[i] = arr[smallest];
		arr[smallest] = temp;

		// Call heapify on the new tree
		heapify(arr, n, smallest);
	}
	return;
}


//services the client requests
void *serviceClient(void *ptr){
	
	FILE *fptr;
	FILE *fr;
	int connfd = *(int *)ptr;
	int read_size, option = 0, result = 0;
	char *sendbuff = (char *)malloc(200);
	char recvbuff[100];
	int size;
	char c;
	int *input = (int *)malloc(sizeof(int)*20);
	char *restring;

	while((read_size = read(connfd, recvbuff, sizeof(recvbuff) - 1)) > 0)
	{
		fptr = fopen("history.txt", "a");
		printf("%s\n", recvbuff);
		fprintf(fptr, "%s\n", recvbuff);
		pthread_mutex_lock(&parse_lock);
		option = parseData(recvbuff, input, &size);
		pthread_mutex_unlock(&parse_lock);
		
		switch(option){
			
			case 1:
				pthread_mutex_lock(&fibonacci_lock);
				result = fibonacci(input[0]);
				pthread_mutex_unlock(&fibonacci_lock);
				printf("\nFibonacci result communicated to the client!\nAwaiting requests...\n");
				sprintf(sendbuff, "Answer: %d\n", result);
				fprintf(fptr, "%s\n", sendbuff);
				write(connfd, sendbuff, strlen(sendbuff));
		    		break;
		    		
		    	case 2:
		    		pthread_mutex_lock(&sort_lock);
		    		restring = heapSort(input, size);
		    		pthread_mutex_unlock(&sort_lock);
		    		printf("\nSorted array communicated to the client!\nAwaiting requests...\n");
		    		sprintf(sendbuff,"Sorted Result: %s\n", restring);
		    		fprintf(fptr, "%s\n", sendbuff);
				write(connfd, sendbuff, strlen(sendbuff));
		    		break;
		    	
		    	case 3:
		    		pthread_mutex_lock(&random_lock);
		    		result = randomgen(input[0], input[1]);
		    		pthread_mutex_unlock(&random_lock);
		    		printf("\nRandom number communicated to the client!\nAwaiting requests...\n");
		    		sprintf(sendbuff, "Random number: %d\n", result);
		    		fprintf(fptr, "%s\n", sendbuff);
				write(connfd, sendbuff, strlen(sendbuff));
		    		break;
		    		
		    	case 4:
		    		puts("Client closed!");
		    		fclose(fptr);
		    		pthread_exit(NULL);
				return;
		    	
		    	case 5:
   				sprintf(sendbuff, "History sent!\n");
   				write(connfd, sendbuff, strlen(sendbuff));
   				break;
   				
   			case 6:
   				sprintf(sendbuff, "Select the valid command!\n");
   				write(connfd, sendbuff, strlen(sendbuff));
   				break;
   			
		    	case 0:	
		    	default:
		    		sprintf(sendbuff,"Invalid Input! Re-enter Command\n");
		    		fprintf(fptr, "%s\n", sendbuff);
		    		write(connfd, sendbuff, strlen(sendbuff));
		    		break;
	    	}
	    	memset(recvbuff, 0, sizeof(recvbuff));
	    	fclose(fptr);
	}
	pthread_exit(NULL);
	return;
}
