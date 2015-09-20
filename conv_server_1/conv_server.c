
/**************************************************************************
 *
 *  CS 6421 - Simple Conversation for ft<->in
 *      It sends out a welcome message when receive connection from client.
 *  Compilation: gcc -o conv_server conv_server.c to compile
 *  Execution:
 *      server: ./conv_server portnum
 *
 *  Students: Phani, Changle
 *
 **************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <inttypes.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <getopt.h>
#include <string.h>
#include <sys/socket.h>


static char* port;
static char* server_ip;
#define BUFSIZE 1024

/*
 * Print a usage message
 */
static void
usage(const char* progname) {
	printf("Usage:  %s portnum\n", progname);
}

/*
 * Converts between two units.
 * from_conv - source unit
 * to_conv - destination unit
 * input_val - value to conv to.
 *
 * return - converted value.
 */
float convert(char from_conv, char to_conv, float input_val) {

	float conv_val = 0.0f;
	switch(from_conv) {
		case 'f' : //f for feet
			switch(to_conv) {
				case 'i' : //i for inches
					conv_val = input_val * 12.0f;
					break;
				default: //feet to any other conv is invalid..
					return -1.0f;
			}
			break;
		case 'i' : //i for inches
			switch(to_conv) {
				case 'f': //f for feet
					conv_val = input_val / 12.0f;
					break;
				default: //inches to any other conv is invalid..
					return -1.0f;
			}
			break;
		default : //only feets and inches are considered.. 
			return -1.0f;
	}
	return conv_val;
}

/*
 *If connection is established then send out welcome message
 */
void
process(int sock)
{
	int n;
	char buf[BUFSIZE] = { '\0' };
	char userInput[BUFSIZE] = { '\0' };
//	char* msg = "Welcome, you're connected to Feet (ft) <-> Inches (in) Conversion server in C\n";
	char* msg = "ft<->in conversion in C\n";

	/* Write a welcome message to the client */
	n = write(sock, msg, strlen(msg));
	if (n < 0){
		perror("ERROR writing to socket");
		close(sock);

		return; //return from this client..
	}

	/* read and print the client's request */
	bzero(buf, BUFSIZE);
	n = read(sock, buf, BUFSIZE);
	if (n < 0){
		perror("ERROR reading from socket");
		close(sock);

		return; //return from this client..
	}
	strcpy(userInput, buf);

	const char s[2] = " "; // space as delimiter
	char *token;
	int tok_count = 0; // number of tokens parsed..
	/* get the first token */
	token = strtok(userInput, s);
	char from_conv[3] = { '\0' }, to_conv[3] = { '\0' }; //capture input and output conversions..
	float input_val = 0.0f; //input value converted to float..

	/* walk through other tokens */
	while( token != NULL ) {
		if(tok_count == 0) { //first token..from conv..
			if(!(strcmp(token, "ft") == 0 || strcmp(token, "in") == 0)) {
				printf("Invalid input[%s]\n", buf);
				close(sock);

				return; //return from this client..
			}
			strcpy(from_conv, token);
			tok_count ++;
		} else if(tok_count == 1) { //second token..to conv..
			if(!(strcmp(token, "ft") == 0 || strcmp(token, "in") == 0)) {
				printf("Invalid input[%s]\n", buf);
				close(sock);

				return; //return from this client..
			}
			strcpy(to_conv, token);
			tok_count ++;
		} else if(tok_count == 2) { //third token..input value..
			//atof doesn't report errors for invalid input.. going with that limitation..
			input_val = atof(token);
			tok_count ++;
		} else { //only 3 tokens supported..
			printf("Invalid input[%s]\n", buf);
			close(sock);

			return; //return from this client..
		}
		token = strtok(NULL, s);
	}
	//if number of tokens aren't matching the protocol.
	if(tok_count != 3) {
		printf("Invalid input[%s]\n", buf);
		close(sock);
		
		return;
	}

	//convert value using input and output units of conversion..
	float conv_val = convert(from_conv[0], to_conv[0], input_val);
	if(conv_val < 0.0f) { //negative is not allowed.. used only for detecting failure..
		printf("Conversion failed..\n");
		close(sock);

		return; //return from this client..
	}
	printf("%.2f %s = %.2f %s\n", input_val, from_conv, conv_val, to_conv);

	bzero(buf, BUFSIZE);
	//reporting precision upto 2 decimal points.
	sprintf(buf, "%.2f\n", conv_val);

	/* Write the converted value to the client */
	n = write(sock, buf, strlen(buf));
	if (n < 0){
		perror("ERROR writing to socket");
		close(sock);

		return; //return from this client..
	}

	close(sock);
}

/*
 * Server
 */
int
server( void )
{
	int optval = 1;
	int sockfd, newsockfd;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	int  n;

	/* First call to socket() function */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

	if (sockfd < 0){
		perror("ERROR opening socket");
		exit(1);
	}
	/* Initialize socket structure */
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(atoi(port));
	/* Now bind the host address using bind() call.*/
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
		perror("ERROR on binding");
		close(sockfd); //cleanup
		exit(1);
	}
	printf("Started server on port %s\n", port);
	/* Listening for the client */
	listen(sockfd,5);
	clilen = sizeof(cli_addr);

	/* Loop forever receiving client connections */
	while(1) {
		/* Accept connection from the client */
		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
		if (newsockfd < 0){
			perror("ERROR on accept");
			close(sockfd); //cleanup.
			exit(1);
		}
		printf("Accepted connection from client[%s]\n", inet_ntoa(cli_addr.sin_addr));
		/* Process a client request on new socket */
		process(newsockfd);
	}

	/*clean up*/
	close(sockfd);

	return 0;
}

int main(int argc, char ** argv){

	const char* progname = argv[0];

	//program name and port number only.
	if (argc != 2){
		usage(progname);
		exit(1);
	}
	port = argv[1];
	//start server..
	if (server() != 0){
		printf("server in trouble\n");
		exit(1);
	}
	return 0;
}
