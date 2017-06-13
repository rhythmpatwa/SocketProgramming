NAME: Rhythm Patwa
E-MAIL ID: rpatwa@uncc.edu

ASSIGNMENT - WRITE A TCP SOCKET BASED APPLICATION IN C:
The assignment requires to implement a client server model. The server is capable of handling multiple client requests simultaneously. The client can request from the server one of the commands from the help menu.

PLATFORM: C
COMPILER: gcc version 5.4.0 20160609

PROGRAM DESIGN:
-There are 2 .c files for the entire implementation:
a)server.c: The server.c file basically opens a socket connection. Once the socket is created the server will open a socket, 			and will listen for incoming requests from the client, the accept() call will accept the connection from the client. Multiple 			clients will have a unique connection descriptor and a new thread is spawned to service every individual client requests. 			There are functions to parse the input from client and functions to service the corresponding command requests. After 			computations, the result is sent over the socket to the client.
b)client.c: The client.c will open a client socket descriptor. After this it will try to connect with the server on the common port number.
		Once the connection has been established with the server the client will run in an infinite loop until the user will 			put in the exit command which will make the client to exit and terminate. The client will wait for input command from the 			user from the available commands and send this command over to the server to be serviced. After sending the command the 		client waits for the response from the server and prints it out to the user.
		
SUMMARY:
The code works perfectly well and meets all the requirements mentioned in the challenge. Multiple client requests are serviced accurately with the help of synchronisation(mutex). 

INSTRUCTIONS FOR THE USER:
By default the code will run from the Command Line.
The user will have to change the addr fields to the localhost address. If the bind fails port number will have to changed.
- To run the server from the default folder, enter the following commands:
	gcc -pthread -o server server.c
-To run the client from the default folder, enter the following commands in a different terminal:
	gcc -o client client.c
- History log will be saved in the file "history.txt" in the same directory.

Output screenshot is provided with filename output_screenshot.png

