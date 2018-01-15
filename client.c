#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include "Card.h"

#define BUFF_SIZE 50

int is_pair(struct Card[]);
int is_two_pair(struct Card[]);
int is_three_kind(struct Card[]);
int is_four_kind(struct Card[]);
int is_straight(struct Card[]);
int is_flush(struct Card[]);
int is_straight_flush(struct Card[]);

int main(int argc, char *argv[])
{
	int myFD;
	long chips = 100;
	struct Card myDeck[52], myHand[4] = {};
	struct sockaddr lname = {AF_UNIX, "Server_Sock"};
	socklen_t llen = sizeof(struct sockaddr) + 12;

	myFD = socket(AF_UNIX, SOCK_STREAM, 0);

	if (myFD == -1)
	{
		perror("Socket() failed");
	}

	if (connect(myFD, &lname, llen) == -1)
	{
		perror("Connect() failed");
	}
	else
	{
		printf("Receiving deck...\n");
		printf("\t received %zd bytes\n", read(myFD, myDeck, 1024));
		close(myFD);

		while (chips > 0)
		{
			if (chips < 10)
			{
				printf("\aWoah there...\n");
				printf("\t You're running out of chips! Only %ld left...\n", chips);
				printf("\a\t tick \t tock\n");
				printf("\a\t\t tick \t tock\n");
			}
			if (count_deck(myDeck, 52) < 4)
			{
				myFD = socket(AF_UNIX, SOCK_STREAM, 0);

				if (myFD == -1)
				{
					perror("Socket() failed");
					exit(EXIT_FAILURE);
				}
				if (connect(myFD, &lname, llen) == -1)
				{
					perror("Connect() failed");
					exit(EXIT_FAILURE);
				}
				printf("\n\t******Out of Cards******\n");
				printf("\t Retrieved a new deck with %zd bytes received.\n", read(myFD, myDeck, 1024));
				close(myFD);
			}
			if (count_deck(myDeck, 52) >= 4)
			{
				char line[256];
				int bet = 0;
				draw(myDeck, myHand);

				printf("Place your chips (%ld in your bank): ", chips);
				fgets(line, sizeof(line), stdin);
				sscanf(line, "%d", &bet);
				while (bet > chips || bet < 1)
				{
					printf("Bet must be affordable and at least 1 chip...");
					printf("Place your bet: ");
					fgets(line, sizeof(line), stdin);
					sscanf(line, "%d", &bet);
				}

				qsort((void *) myHand, 4, sizeof(myDeck[0]), comparator);
				show_deck(myHand, 4);

				if (is_straight(myHand) == 1)
				{
					printf("\n\a Straight!!\n");
					chips += (bet * 98);
				}
				else if (is_pair(myHand))
				{
					if (is_two_pair(myHand) == 1)
					{
						printf("\t\a Two Pair!\n");
						chips += (bet * 36);
					}
					else
					{
						printf("\t\a Pair\n");
						chips += (bet * 3);
					}
				}
				else if (is_three_kind(myHand) == 1)
				{
					if (is_four_kind(myHand) == 1)
					{
						printf("\t\a !!    F o u r  o f  a  K i n d    !!\n");
						printf("\t\a !! J A C K P O T ~~ J A C K P O T !!\n");
						chips += (bet * 20825);
					}
					else
					{
						printf("\t\a Three of a Kind!!!\n");
						chips += (bet * 108);
					}

				}
				else if (is_flush(myHand) == 1)
				{
					if (is_straight_flush(myHand) == 1)
					{
						printf("\t\a !! STRAIGHT FLUSH !!\n");
						chips += (bet * 6153);
					}
					else
					{
						printf("\t\a Flush!\n");
						chips += (bet * 96);
					}
				}
				else
				{
					chips -= bet;
				}
			}
			shuffle_deck(myDeck);
		}
	}
}

/*
 * These are really ugly BUT they work and I'm on a
 *
 *                T I M E  C R U N C H
 *
 * Please except my apologies for the painful reading.
 */

int is_straight_flush(struct Card hand[])
{
	if (is_flush(hand) == is_straight(hand))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int is_flush(struct Card hand[])
{
	if (hand[0].suit == hand[1].suit && hand[1].suit == hand[2].suit && hand[2].suit == hand[3].suit)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int is_four_kind(struct Card hand[])
{
	if (hand[0].value == hand[1].value && hand[1].value == hand[2].value && hand[2].value == hand[3].value)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int is_three_kind(struct Card hand[])
{
	if (hand[0].value == hand[1].value && hand[1].value == hand[2].value)
	{
		return 1;
	}
	if (hand[1].value == hand[2].value && hand[2].value == hand[3].value)
	{
		return 1;
	}
	return 0;
}

int is_two_pair(struct Card hand[])
{
	if (is_pair(hand) != 1)
	{
		return 0;
	}
	if ((hand[0].value == hand[1].value) && (hand[2].value != hand[3].value))
	{
		return 0;
	}
	if (hand[1].value == hand[2].value)
	{
		return 0;
	}
	if ((hand[0].value != hand[1].value) && (hand[2].value == hand[3].value))
	{
		return 0;
	}
	return 1;
}

int is_pair(struct Card hand[])
{
	int index1, index2;
	for (index1 = 0; index1 < 3; index1++)
	{
		for (index2 = index1 + 1; index2 < 4; index2++)
		{
			if (hand[index1].value == hand[index2].value)
			{
				return 1;
			}
		}
	}
	return 0;
}

int is_straight(struct Card hand[])
{
	if (hand[0].suit == hand[1].suit)
	{
		if (hand[1].suit == hand[2].suit)
		{
			if (hand[2].suit == hand[3].suit)
			{
				return 0;
			}
		}
	}
	else
	{
		if (hand[0].value == (hand[1].value - 1))
		{
			if (hand[1].value == (hand[2].value - 1))
			{
				if (hand[2].value == (hand[3].value - 1))
				{
					return 1;
				}
				else
				{
					return 0;
				}
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
}
