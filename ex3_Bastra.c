#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --- DECLARING THE GLOBAL VARIABLES AND CLASSES ---

#define DECK_SIZE 52

// we use NULL_SUIT and NULL_VALUE so that we can check if the card is playable in the is_card_playable function
// also their values will be equal to the number of elements, other than them (playable elements)
// for example, NULL_SUIT = 4, and the number of suits in the game is really 4.
enum Suit { SPADES, HEARTS, DIAMONDS, CLUBS, NULL_SUIT };
enum Value { ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, NULL_VALUE };

typedef struct {
    enum Suit suit;
    enum Value value;
} card;

// these are used for printing the cards
const char *SUIT_NAMES[NULL_SUIT] = {
    "SPADES", "HEARTS", "DIAMONDS", "CLUBS"
};
const char *VALUE_NAMES[NULL_VALUE] = {
    "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King"
};



// --- FUNCTIONS ---



// fill the deck with one copy of each card
void create_deck(card deck[DECK_SIZE]) {
    int index = 0;
    for (int s = 0; s < NULL_SUIT; s++) {
        for (int v = 0; v < NULL_VALUE; v++) {
            deck[index].suit = (enum Suit)s;
            deck[index].value = (enum Value)v;
            index++;
        }
    }
}


// we swap places of random card from the deck and the first card, then we do the same for the second, etc
void shuffle_deck(card deck[DECK_SIZE]) {
    // sets the seed to rand() functions so that every time it will use computer's time as the seed and so will be different
    srand(time(NULL));
    for (int i = DECK_SIZE - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        card temporary_card_placeholder = deck[i];
        deck[i] = deck[j];
        deck[j] = temporary_card_placeholder;
    }
}


// we check if the card is playable (not null), it will return 1 if yes and 0 if no
int is_card_playable(const card card) {
    if (card.suit >= NULL_SUIT || card.value >= NULL_VALUE) {
        return 0;
    }
    return 1;
}


// check if the deck is empty so that we could use it when the game is over
int is_deck_empty(card deck[DECK_SIZE]) {
    for (int i = 0; i < DECK_SIZE; i++) {
        if (is_card_playable(deck[i])) {
            return 0;
        }
    }
    return 1;
}


// deal 4 cards to all players and the board
void deal_cards(card deck[DECK_SIZE], card player1[DECK_SIZE], card player2[DECK_SIZE], card player3[DECK_SIZE], card player4[DECK_SIZE], card board[DECK_SIZE]) {
    int card_index = 0;
    for (int i = 0; i < 4; i++) {

        // player 1
        player1[i] = deck[card_index];
        for (int j = card_index; j < DECK_SIZE - 1; j++) {
            deck[j] = deck[j + 1];  // shift cards left
        }
        deck[DECK_SIZE - 1].suit = NULL_SUIT;  // clear the last card
        deck[DECK_SIZE - 1].value = NULL_VALUE;
        card_index++;

        // player 2
        player2[i] = deck[card_index];
        for (int j = card_index; j < DECK_SIZE - 1; j++) {
            deck[j] = deck[j + 1];
        }
        deck[DECK_SIZE - 1].suit = NULL_SUIT;
        deck[DECK_SIZE - 1].value = NULL_VALUE;
        card_index++;

        // player 3
        player3[i] = deck[card_index];
        for (int j = card_index; j < DECK_SIZE - 1; j++) {
            deck[j] = deck[j + 1];
        }
        deck[DECK_SIZE - 1].suit = NULL_SUIT;
        deck[DECK_SIZE - 1].value = NULL_VALUE;
        card_index++;

        // player 4
        player4[i] = deck[card_index];
        for (int j = card_index; j < DECK_SIZE - 1; j++) {
            deck[j] = deck[j + 1];
        }
        deck[DECK_SIZE - 1].suit = NULL_SUIT;
        deck[DECK_SIZE - 1].value = NULL_VALUE;
        card_index++;

        // board
        if (deck[card_index].value == JACK) {
            // if it's a jack, moves it to the end of the deck
            for (int j = card_index; j < DECK_SIZE - 1; j++) {
                deck[j] = deck[j + 1];
            }
            deck[DECK_SIZE - 1] = deck[card_index];  // place jack at the end of the deck
        } else {
            board[i] = deck[card_index];
        }

        // shift cards after the board is dealt to
        for (int j = card_index; j < DECK_SIZE - 1; j++) {
            deck[j] = deck[j + 1];
        }
        deck[DECK_SIZE - 1].suit = NULL_SUIT;
        deck[DECK_SIZE - 1].value = NULL_VALUE;
        card_index++;
    }
}


card draw_card(card deck[DECK_SIZE]) {
    for (int i = 0; i < DECK_SIZE; i++) {
        if (is_card_playable(deck[i])) {
            card drawn_card = deck[i];
            deck[i].suit = NULL_SUIT;
            deck[i].value = NULL_VALUE;
            return drawn_card;
        }
    }
    return (card){NULL_SUIT, NULL_VALUE};
}


void display_hand(card player[], int hand_size) {
    printf("\nYour have:\n");
    if (hand_size == 0) {
        printf("\nYou have no cards left.\n");
        return;
    }

    for (int i = 0; i < hand_size; i++) {
        if (is_card_playable(player[i])) {
            printf("%d. %s of %s\n", i + 1, VALUE_NAMES[player[i].value], SUIT_NAMES[player[i].suit]);
        }
    }
    printf("\n");
}


// remove the card from the hand and shift remaining cards left so that there is no gaps in between
void remove_card_from_hand(card player[], int *hand_size, int card_index, card *deck) {
    for (int i = card_index; i < *hand_size - 1; i++) {
        player[i] = player[i + 1];  // Shift the cards
    }

    player[*hand_size - 1].suit = NULL_SUIT;
    player[*hand_size - 1].value = NULL_VALUE;
    (*hand_size)--;
    if (*hand_size == 0) {
      for (int i = 0; i < 4; i++) {
        player[i] = draw_card(deck); // if the hand is empty, the player should take 4 cards from the deck
      }
    (*hand_size) = 4;
    }
}


void take_card_from_board(card player[], card board[], int *hand_size, const card *taken_card, int *board_size) {
    printf("\nYou took %s of %s from board\n", VALUE_NAMES[taken_card->value], SUIT_NAMES[taken_card->suit]);
    player[*hand_size] = *taken_card;
    (*hand_size)++;
    for (int i = 0; i < *board_size; i++) {
        if (board[i].value == taken_card->value && board[i].suit == taken_card->suit) {
            // shift board to the left to match indexes
            for (int j = i; j < *board_size - 1; j++) {
                board[j] = board[j + 1];
            }
            board[*board_size - 1].value = NULL_VALUE;
            board[*board_size - 1].suit = NULL_SUIT;
            (*board_size)--;
            break;
        }
    }
}


void display_board(const int board_size, const card *board) {
    printf("\nOn board:\n");
    for (int i = 0; i < board_size; i++) {
        if (is_card_playable(board[i])) {
            printf("%s of %s\n", VALUE_NAMES[board[i].value], SUIT_NAMES[board[i].suit]);
        }
    }
}


card suggest_best_move(card bot[], int hand_size, card board[], const int board_size) {
    display_hand(bot, hand_size);

    // the player that collects most cards receives 3 points, so jack will give us more points
    // if there is another jack on the board, it will be 4 points, so it's more in priority than 10 of diamonds
    // but we need to check if we can make bastra first

    int is_jack_on_hand = 0;
    int jack_index = -1;

    for (int i = 0; i < hand_size; i++) {
        if (bot[i].value == JACK) {
            is_jack_on_hand = 1;
            jack_index = i;
        }
    }

    // bastra check (10 points gain)
    // 9 => 9, 4, 5, 3, 6 = bastra
    int possible_taken_cards = 0;
    for (int i = 0; i < hand_size; i++) {
        if (bot[i].value == KING || bot[i].value == QUEEN || bot[i].value == JACK) {
            continue;
        }
        for (int j = 0; j < board_size; j++) {
            if (bot[i].value == board[j].value) {
                possible_taken_cards++;
            }
            for (int z = j; z < board_size; z++) {
                if (bot[i].value - board[j].value == board[z].value) {
                    possible_taken_cards+=2;
                    break;
                }
            }
        }
        if (possible_taken_cards >= board_size) {
            printf("Bot played: %s of %s\n", VALUE_NAMES[bot[i].value], SUIT_NAMES[bot[i].suit]);
            return bot[i];
        }
        // if the bastra is not possible, then we should play with jack to get 3/4 points
        if (is_jack_on_hand && (jack_index != -1)) {
            printf("Bot played: %s of %s\n", VALUE_NAMES[bot[jack_index].value], SUIT_NAMES[bot[jack_index].suit]);
            return bot[jack_index];
        }
        // if the number of cards that can be taken is bigger than half of the board and the board is big enough, then the player will get most cards this round, so 3 points gain still
        if (possible_taken_cards>(board_size/2+1) && board_size >2) {
            printf("Bot played: %s of %s\n", VALUE_NAMES[bot[i].value], SUIT_NAMES[bot[i].suit]);
            return bot[i];
        }
    }

    // 3 points gain
    for (int i = 0; i < hand_size; i++) {
        if (bot[i].value == 10) {
            for (int j = 0; j < board_size; j++) {
                if (board[j].value == 10 && board[j].suit == DIAMONDS) {
                    printf("Bot played: %s of %s\n", VALUE_NAMES[bot[i].value], SUIT_NAMES[bot[i].suit]);
                    return bot[i];
                }
            }
        }
    }

    // 2 points gain
    for (int i = 0; i < hand_size; i++) {
        if (bot[i].value == 2) {
            for (int j = 0; j < board_size; j++) {
                if (board[j].value == 2 && board[j].suit == CLUBS) {
                    printf("Bot played: %s of %s\n", VALUE_NAMES[bot[i].value], SUIT_NAMES[bot[i].suit]);
                    return bot[i];
                }
            }
        }
    }

    // 1 points gain
    for (int i = 0; i < hand_size; i++) {
        if (bot[i].value == 1) {
            for (int j = 0; j < board_size; j++) {
                if (board[j].value == 1) {
                    printf("Bot played: %s of %s\n", VALUE_NAMES[bot[i].value], SUIT_NAMES[bot[i].suit]);
                    return bot[i];
                }
            }
        }
    }

    // put card to take something from the board
    for (int i = 0; i < hand_size; i++) {
        for (int j = 0; j < board_size; j++) {
            if (bot[i].value == board[j].value) {
                printf("Bot played: %s of %s\n", VALUE_NAMES[bot[i].value], SUIT_NAMES[bot[i].suit]);
                return bot[i];
            }
        }
    }

    // if there is no such situations, just put the random card
    printf("Bot played: %s of %s\n", VALUE_NAMES[bot[rand()%hand_size].value], SUIT_NAMES[bot[rand()%hand_size].suit]);
    return bot[rand()%hand_size];
}



card player_turn(card player[], const int *player_size) {
    // choose a card to play
    int chosen_card_index;
    int valid_move = 0;
    display_hand(player, *player_size);
    while (!valid_move) {
        printf("Choose a card to play:\n");
        scanf("%d", &chosen_card_index);
        if (chosen_card_index < 1 || chosen_card_index > *player_size || !is_card_playable(player[chosen_card_index-1])) {
            printf("You can't choose it, please try again\n");
        }
        else {
            valid_move = 1;
        }
    }
    const card played_card = player[chosen_card_index-1];
    printf("You played: %s of %s\n", VALUE_NAMES[played_card.value], SUIT_NAMES[played_card.suit]);

    return played_card;
}



card *take_action(card player[], int *player_size, card deck[DECK_SIZE], card board[DECK_SIZE], int *board_size, const card played_card) {
    // start to put taken cards aside in the array to deal with them later
    card taken_cards[DECK_SIZE];
    int number_of_taken_cards = 0;
	if (played_card.value == 10) {
	    // jacks value is 10
	    printf("\nYou played jack! All cards are taken");
		for (int card_index = 0; card_index < *board_size; card_index++) {
		    taken_cards[number_of_taken_cards] = board[card_index];
		    number_of_taken_cards++;
		}
	} else {
	    for (int card_index = 0; card_index < *board_size; card_index++) {
	        if (played_card.value == board[card_index].value) {
	            printf("\nThere is a card with the same value as your played card\n");
	            taken_cards[number_of_taken_cards] = board[card_index];
	            number_of_taken_cards++;
	        } else if (played_card.value != 11 && played_card.value != 12) {
	            // queen = 11 and king = 12
	            // face cards cannot take part in pairing match, only numbers
	            // we dont include jack because it is mentioned in the prev condition
	            const int difference = played_card.value-board[card_index].value;
	            if (difference > 0) {
	                // we start from card_index+1 to avoid duplication of checking
	                for (int compared_card_index = card_index+1; compared_card_index < *board_size; compared_card_index++) {
	                    if (board[compared_card_index].value == difference-1) {
	                        printf("\n%s of %s and %s of %s\n", VALUE_NAMES[board[card_index].value], SUIT_NAMES[board[card_index].suit], VALUE_NAMES[board[compared_card_index].value], SUIT_NAMES[board[compared_card_index].suit]);
	                        // for example: we put 3 of diamonds, there is a card with value 1 on the board
	                        // we will check if there is a card with value 3-1=2
	                        // if yes, we take 1 and 2, which sums to 3

	                        taken_cards[number_of_taken_cards] = board[card_index];
	                        number_of_taken_cards++;
	                        taken_cards[number_of_taken_cards] = board[compared_card_index];
	                        number_of_taken_cards++;
	                    }
	                }
	            }
	        }
	    }
	}
    for (int i = 0; i < number_of_taken_cards; i++) {
        for (int j = 0; j < *board_size; j++) {
            if (board[j].value == taken_cards[i].value && board[j].suit == taken_cards[i].suit) {
                printf("");
                take_card_from_board(player, board, player_size, &taken_cards[i], board_size);
            }
        }
    }
    if (number_of_taken_cards == 0) {
        printf("No cards are taken - played card goes to board: %s of %s\n", VALUE_NAMES[played_card.value], SUIT_NAMES[played_card.suit]);
        board[*board_size] = played_card;
        (*board_size)++;

        // we find the card in the player's hand and remove it
        for (int i = 0; i < *player_size; i++) {
            if (player[i].value == played_card.value && player[i].suit == played_card.suit) {
                remove_card_from_hand(player, player_size, i, deck);
                break;
            }
        }
    }


    if (!is_deck_empty(deck)) {
        player[*player_size] = draw_card(deck);
        (*player_size)++;
    }
    return &taken_cards[*board_size];
}


int calculate_points(card taken_cards[], int hand_size) {
    int points = 0;
    for (int i = 0; i < hand_size; i++) {
        // jacks and aces are 1 point each
        if (taken_cards[i].value == JACK || taken_cards[i].value == 0) {
            points += 1;
        }
        // 2 clubs = 2 points
        if (taken_cards[i].value == 1 && taken_cards[i].suit == CLUBS) {
            points += 2;
        }
        // ten diamonds = 3 point
        if (taken_cards[i].value == 9 && taken_cards[i].suit == DIAMONDS) {
            points += 3;
        }
    }
    return points;
}


int points_checking(int points[], card deck[DECK_SIZE]) {
    int max_points = points[0];
    int winner = 1;

    for (int i = 1; i < 4; i++) {
        if (points[i] > max_points) {
            max_points = points[i];
            winner = i + 1; // +1 because player index starts from 0
        }
    }
    printf("\nPlayer %d has max points at the moment: %d points!\n", winner, max_points);
    if (max_points >=101) {
      printf("We have the winner!\n");
      return 1;
    }
    return 0;
}


void play_game() {
    card deck[DECK_SIZE];
    card player1[DECK_SIZE], player2[DECK_SIZE], player3[DECK_SIZE], player4[DECK_SIZE];
    card board[DECK_SIZE];

    int player1_size = 4, player2_size = 4, player3_size = 4, player4_size = 4;
    int board_size = 4;
    int player1_points = 0, player2_points = 0, player3_points = 0, player4_points = 0;

    card *players_list[4] = { player1, player2, player3, player4 };
    int scores[4] = { player1_points,  player2_points, player3_points, player4_points };
    int hand_sizes[4] = { player1_size, player2_size, player3_size, player4_size };

    // put time as a seed to make rand more random
    srand(time(NULL));

    create_deck(deck);
    shuffle_deck(deck);

    deal_cards(deck, player1, player2, player3, player4, board);

    do {
        for (int i = 0; i < 4; i++) {
            if (i%2 == 0) {
                printf("\nPlayer %d [human] is playing\n", i+1);
                display_board(board_size, board);
                card played_card = player_turn(players_list[i], &hand_sizes[i]);
                take_action(players_list[i], &hand_sizes[i], deck, board, &board_size, played_card);
            }
            else {
                printf("\nPlayer %d [bot] is playing\n", i+1);
                display_board(board_size, board);
                card played_card = suggest_best_move(players_list[i], hand_sizes[i], board, board_size);
                take_action(players_list[i], &hand_sizes[i], deck, board, &board_size, played_card);
            }
            // checks if the game is over
            if (is_deck_empty(deck) && (hand_sizes[0] == 0 || hand_sizes[1] == 0 || hand_sizes[2] == 0 || hand_sizes[3] == 0)) {
                printf("\nThe game is over!\n");
                break;
            }
        }
        for (int i = 0; i < 4; i++) {
            scores[i] += calculate_points(players_list[i], hand_sizes[i]);
        }
    } while (!points_checking(scores, deck));
}

int main(void) {
	play_game();
	return 0;
}
