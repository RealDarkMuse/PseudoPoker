#include <stdlib.h>
#include <stdio.h>
#include "Card.h"

void shuffle_deck(struct Card deck[])
{
	int index, entropy, r;
	struct Card t_card;
	for (entropy = 0; entropy < 5; entropy++)
	{
		for (index = 0; index < 52; index++)
		{
			r = (rand() % 51) + 1;
			t_card = deck[index];
			deck[index] = deck[r];
			deck[r] = t_card;
		}
	}
}

void show_deck(struct Card deck[], int size)
{
	int iter;
	for (iter = 0; iter < size; iter++)
	{
		render_card(deck[iter]);
		printf("\n");
	}
}

void render_card(struct Card card)
{
	char *value_name, *suit_name;
	switch (card.value)
	{
		case 1:
			value_name = "Ace";
			break;
		case 2:
			value_name = "Two";
			break;
		case 3:
			value_name = "Three";
			break;
		case 4:
			value_name = "Four";
			break;
		case 5:
			value_name = "Five";
			break;
		case 6:
			value_name = "Six";
			break;
		case 7:
			value_name = "Seven";
			break;
		case 8:
			value_name = "Eight";
			break;
		case 9:
			value_name = "Nine";
			break;
		case 10:
			value_name = "Ten";
			break;
		case 11:
			value_name = "Jack";
			break;
		case 12:
			value_name = "Queen";
			break;
		case 13:
			value_name = "King";
			break;
		default:
			value_name = "N/A";
			break;
	}

	switch (card.suit)
	{
		case 'c':
			suit_name = "Clubs";
			break;
		case 'd':
			suit_name = "Diamonds";
			break;
		case 'h':
			suit_name = "Hearts";
			break;
		case 's':
			suit_name = "Spades";
			break;
		default:
			suit_name = "N/A";
			break;
	}
	printf("[%s of %s]", value_name, suit_name);
}

void fill_deck(struct Card deck[])
{
	// Fill the deck!
	int iter;

	for (iter = 0; iter < 52; iter++)
	{
		if (iter < 13)  // first 12 cards are clubs
		{
			deck[iter].value = (unsigned) iter + 1;
			deck[iter].suit = 'c';
		}
		else if (iter < 26)
		{
			deck[iter].value = (unsigned) iter - 12;
			deck[iter].suit = 'd';
		}
		else if (iter < 39)
		{
			deck[iter].value = (unsigned) iter - 25;
			deck[iter].suit = 'h';
		}
		else if (iter < 52)
		{
			deck[iter].value = (unsigned) iter - 38;
			deck[iter].suit = 's';
		}
		else
		{
			puts("We have more than 52 cards...");
			exit(EXIT_FAILURE);
		}
	}
}

int count_deck(struct Card deck[], int size)
{
	int cards = 0, index;
	for (index = 0; index < size; index++)
	{
		if (deck[index].value != 0)
		{
			cards++;
		}
	}
	return cards;
}

void draw(struct Card deck[], struct Card hand[])
{
	int d_iter, h_iter;
	if (count_deck(hand, 4) != 0)  // given a full hand...
	{
		for (h_iter = 0; h_iter < 4; h_iter++)
		{
			hand[h_iter].suit = 'E';
			hand[h_iter].value = 0;
		}
	}

	for (h_iter = 0, d_iter = 0; d_iter < 52, h_iter < 4; d_iter++)
	{
		if (deck[d_iter].value != 0)
		{
			hand[h_iter].value = deck[d_iter].value;
			hand[h_iter].suit = deck[d_iter].suit;
			deck[d_iter].value = 0;
			deck[d_iter].suit = 'E';
			h_iter++;
		}
	}
}

int comparator(const void *p, const void *q)
{
	int l = ((struct Card *) p)->value;
	int r = ((struct Card *) q)->value;
	return (l - r);
}