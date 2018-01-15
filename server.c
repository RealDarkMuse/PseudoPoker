#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <signal.h>
#include "Card.h"

int myfd, new_fd;
struct sigaction news;
struct sigaction olds;

void terminate(int);

int main(int argc, char *argv[])
{
	int repeat = 1;

	struct Card deck[52];
	struct sockaddr myname = {AF_UNIX, "Server_Sock"}, peername = {AF_UNIX, ""};

	socklen_t mylen = sizeof(struct sockaddr) + 5, peerlen = sizeof(struct sockaddr) + 1;

	news.sa_handler = terminate;
	news.sa_flags = 0;
	sigaction(SIGINT, &news, &olds);

	srand(time(NULL));

	myfd = socket(AF_UNIX, SOCK_STREAM, 0);

	if (myfd == -1)
	{
		perror("Socket() failed");
	}

	if (bind(myfd, &myname, mylen) == -1)
	{
		perror("Failed to bind() myfd to name with given length");
	}
	do
	{
		if (listen(myfd, 1) == -1)
		{
			perror("Listen() failed");
		}

		new_fd = accept(myfd, &peername, &peerlen);
		if (new_fd == -1)
		{
			perror("Accept() failed");
		}
		else
		{
			printf("Listener has accepted connection from %s.\n", peername.sa_data);
		}
		fill_deck(deck);
		shuffle_deck(deck);
		printf("Sending the deck...\n");
		printf("\t%zd bytes sent.\n", write(new_fd, deck, sizeof(struct Card) * 52));
	} while (repeat == 1);

	return 0;
}

void terminate(int signo)
{
	if (signo == SIGINT)
	{
		sigaction(SIGINT, &olds, NULL);
		printf("Closing connection...\n");
		close(myfd);
		close(new_fd);
		unlink("Server_Sock");
		printf("Completed.\n");
		exit(EXIT_SUCCESS);
	}
	else
	{
		puts("Not a ctrl+c....\n\twe're not going to do anything.");
	}
}