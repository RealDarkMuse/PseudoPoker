//
// Created by Admin on 12/13/2017.
//

#ifndef PROGRAM9_CARD_H
#define PROGRAM9_CARD_H

#endif //PROGRAM9_CARD_H

struct Card
{
	unsigned int value;
	char suit;
};

void shuffle_deck(struct Card[]);
void show_deck(struct Card[], int);
void render_card(struct Card);
void fill_deck(struct Card[]);
int count_deck(struct Card[], int);
void draw(struct Card[], struct Card[]);

int comparator(const void *, const void *);