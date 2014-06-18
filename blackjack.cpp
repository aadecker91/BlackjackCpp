/*
 * Author: Adam Decker
 * Date: 5/28/2014
 * Description: This program will play a perfect game of blackjack against the
 * house and record the results and how certain changes affect the player's
 * winnings/losses
 */

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
using namespace std;

const string SPD = "Spade";  // keywords simply used to make the code shorter
const string HRT = "Heart";
const string DIA = "Diamond";
const string CLB = "Club";

// The card data structure
struct card {
	unsigned int r; 	// The rank value of the card
	string s; 			// The suit of the card
	string n;			// The name of the card
};

// Variables used
const int players_bet = 10; 		// The player's base/minimum bet
int players_bank = 10000; 	// The player's total amount of money
bool playerBlackjack;		// Used to indicate player Blackjack
bool houseBlackjack;		// Used to indicate house Blackjack
bool insurance;				// Used to indicate house upcard of an ACE
bool playerDouble;			// Used to indicate a player doubling down
bool playerSurrender;		// Used to indicate a player surrendering
bool firstHand;				// Used to indicate a a freshly dealt hand
vector<card> players_hand; 			// The player's hand
bool playerSoftHand;	//indicates the player has a soft hand
vector<card> house_hand; 	// The houses hand
bool houseSoftHand;		//indicates the house has a soft hand
vector<card> deck;		// The deck used to deal cards

//function declarations
void shuffle(vector<card> &deck); //used to reshuffle the deck
void dealCards(vector<card> &deck, vector<card> &players_hand, vector<card> &house_hand); // Deals the cards to the player and the house
void hit(vector<card> &players_hand); //add's one card to a hand (either the player's or the house's
void doubleDown(vector<card> &players_hand); // Double down: Player gets one more card and they double their bet
void surrender(vector<card> &players_hand);  //Surrender the hand to the house, lose half of original bet
void playerLogic(vector<card> &players_hand); // The logic the player will follow
void houseLogic(vector<card> &house_hand);
void play(); // Plays a single perfect game for the player

int main( int argc, char ** argv ) {
	srand(time(NULL));
	string command = "";

	cin >> command;
	while(command != "q") {
		play();

		cout << players_bank << endl;

		cout << "Your hand is: " << endl;
		for(unsigned i = 0; i < players_hand.size(); i++){
			cout << players_hand[i].r << " of " << players_hand[i].s << endl;
		}
		cout << "The houses hand is:" << endl;
		for(unsigned i = 0; i < house_hand.size(); i++){
			cout << house_hand[i].r << " of " << house_hand[i].s << endl;
		}
		cin >> command;
	}
	return 0;
}
// Resets the deck and the house/player hands
void shuffle(vector<card> &deck) {
	// Erase both the player's and the house's hand
	players_hand.erase(players_hand.begin(), players_hand.end());
	house_hand.erase(house_hand.begin(), house_hand.end());
	deck = {
		{ 1, SPD, "Ace" }, { 2, SPD, "Two" }, { 3, SPD, "Three" }, { 4, SPD, "Four" }, { 5, SPD, "Five" }, { 6, SPD, "Six" }, { 7, SPD, "Seven" },
		{ 8, SPD, "Eight" }, { 9, SPD, "Nine" }, { 10, SPD, "Ten" }, { 10, SPD, "Jack" }, { 10, SPD, "Queen" }, { 10, SPD, "King" },
		{ 1, HRT, "Ace" }, { 2, HRT, "Two" }, { 3, HRT, "Three" }, { 4, HRT, "Four" }, { 5, HRT, "Five" }, { 6, HRT, "Six" }, { 7, HRT, "Seven" },
		{ 8, HRT, "Eight" }, { 9, HRT, "Nine" }, { 10, HRT, "Ten" }, { 10, HRT, "Jack" }, { 10, HRT, "Queen" }, { 10, HRT, "King" },
		{ 1, DIA, "Ace" }, { 2, DIA, "Two" }, { 3, DIA, "Three" }, { 4, DIA, "Four" }, { 5, DIA, "Five" }, { 6, DIA, "Six" }, { 7, DIA, "Seven" },
		{ 8, DIA, "Eight" }, { 9, DIA, "Nine" }, { 10, DIA, "Ten" }, { 10, DIA, "Jack" }, { 10, DIA, "Queen" }, { 10, DIA, "King" },
		{ 1, CLB, "Ace" }, { 2, CLB, "Two" }, { 3, CLB, "Three" }, { 4, CLB, "Four" }, { 5, CLB, "Five" }, { 6, CLB, "Six" }, { 7, CLB, "Sever" },
		{ 8, CLB, "Eight" }, { 9, CLB, "Nine" }, { 10, CLB, "Ten" }, { 10, CLB, "Jack" }, { 10, CLB, "Queen" }, { 10, CLB, "King" },
		{ 1, SPD, "Ace" }, { 2, SPD, "Two" }, { 3, SPD, "Three" }, { 4, SPD, "Four" }, { 5, SPD, "Five" }, { 6, SPD, "Six" }, { 7, SPD, "Seven" },
		{ 8, SPD, "Eight" }, { 9, SPD, "Nine" }, { 10, SPD, "Ten" }, { 10, SPD, "Jack" }, { 10, SPD, "Queen" }, { 10, SPD, "King" },
		{ 1, HRT, "Ace" }, { 2, HRT, "Two" }, { 3, HRT, "Three" }, { 4, HRT, "Four" }, { 5, HRT, "Five" }, { 6, HRT, "Six" }, { 7, HRT, "Seven" },
		{ 8, HRT, "Eight" }, { 9, HRT, "Nine" }, { 10, HRT, "Ten" }, { 10, HRT, "Jack" }, { 10, HRT, "Queen" }, { 10, HRT, "King" },
		{ 1, DIA, "Ace" }, { 2, DIA, "Two" }, { 3, DIA, "Three" }, { 4, DIA, "Four" }, { 5, DIA, "Five" }, { 6, DIA, "Six" }, { 7, DIA, "Seven" },
		{ 8, DIA, "Eight" }, { 9, DIA, "Nine" }, { 10, DIA, "Ten" }, { 10, DIA, "Jack" }, { 10, DIA, "Queen" }, { 10, DIA, "King" },
		{ 1, CLB, "Ace" }, { 2, CLB, "Two" }, { 3, CLB, "Three" }, { 4, CLB, "Four" }, { 5, CLB, "Five" }, { 6, CLB, "Six" }, { 7, CLB, "Sever" },
		{ 8, CLB, "Eight" }, { 9, CLB, "Nine" }, { 10, CLB, "Ten" }, { 10, CLB, "Jack" }, { 10, CLB, "Queen" }, { 10, CLB, "King" },
		{ 1, SPD, "Ace" }, { 2, SPD, "Two" }, { 3, SPD, "Three" }, { 4, SPD, "Four" }, { 5, SPD, "Five" }, { 6, SPD, "Six" }, { 7, SPD, "Seven" },
		{ 8, SPD, "Eight" }, { 9, SPD, "Nine" }, { 10, SPD, "Ten" }, { 10, SPD, "Jack" }, { 10, SPD, "Queen" }, { 10, SPD, "King" },
		{ 1, HRT, "Ace" }, { 2, HRT, "Two" }, { 3, HRT, "Three" }, { 4, HRT, "Four" }, { 5, HRT, "Five" }, { 6, HRT, "Six" }, { 7, HRT, "Seven" },
		{ 8, HRT, "Eight" }, { 9, HRT, "Nine" }, { 10, HRT, "Ten" }, { 10, HRT, "Jack" }, { 10, HRT, "Queen" }, { 10, HRT, "King" },
		{ 1, DIA, "Ace" }, { 2, DIA, "Two" }, { 3, DIA, "Three" }, { 4, DIA, "Four" }, { 5, DIA, "Five" }, { 6, DIA, "Six" }, { 7, DIA, "Seven" },
		{ 8, DIA, "Eight" }, { 9, DIA, "Nine" }, { 10, DIA, "Ten" }, { 10, DIA, "Jack" }, { 10, DIA, "Queen" }, { 10, DIA, "King" },
		{ 1, CLB, "Ace" }, { 2, CLB, "Two" }, { 3, CLB, "Three" }, { 4, CLB, "Four" }, { 5, CLB, "Five" }, { 6, CLB, "Six" }, { 7, CLB, "Sever" },
		{ 8, CLB, "Eight" }, { 9, CLB, "Nine" }, { 10, CLB, "Ten" }, { 10, CLB, "Jack" }, { 10, CLB, "Queen" }, { 10, CLB, "King" },
		{ 1, SPD, "Ace" }, { 2, SPD, "Two" }, { 3, SPD, "Three" }, { 4, SPD, "Four" }, { 5, SPD, "Five" }, { 6, SPD, "Six" }, { 7, SPD, "Seven" },
		{ 8, SPD, "Eight" }, { 9, SPD, "Nine" }, { 10, SPD, "Ten" }, { 10, SPD, "Jack" }, { 10, SPD, "Queen" }, { 10, SPD, "King" },
		{ 1, HRT, "Ace" }, { 2, HRT, "Two" }, { 3, HRT, "Three" }, { 4, HRT, "Four" }, { 5, HRT, "Five" }, { 6, HRT, "Six" }, { 7, HRT, "Seven" },
		{ 8, HRT, "Eight" }, { 9, HRT, "Nine" }, { 10, HRT, "Ten" }, { 10, HRT, "Jack" }, { 10, HRT, "Queen" }, { 10, HRT, "King" },
		{ 1, DIA, "Ace" }, { 2, DIA, "Two" }, { 3, DIA, "Three" }, { 4, DIA, "Four" }, { 5, DIA, "Five" }, { 6, DIA, "Six" }, { 7, DIA, "Seven" },
		{ 8, DIA, "Eight" }, { 9, DIA, "Nine" }, { 10, DIA, "Ten" }, { 10, DIA, "Jack" }, { 10, DIA, "Queen" }, { 10, DIA, "King" },
		{ 1, CLB, "Ace" }, { 2, CLB, "Two" }, { 3, CLB, "Three" }, { 4, CLB, "Four" }, { 5, CLB, "Five" }, { 6, CLB, "Six" }, { 7, CLB, "Sever" },
		{ 8, CLB, "Eight" }, { 9, CLB, "Nine" }, { 10, CLB, "Ten" }, { 10, CLB, "Jack" }, { 10, CLB, "Queen" }, { 10, CLB, "King" },
		{ 1, SPD, "Ace" }, { 2, SPD, "Two" }, { 3, SPD, "Three" }, { 4, SPD, "Four" }, { 5, SPD, "Five" }, { 6, SPD, "Six" }, { 7, SPD, "Seven" },
		{ 8, SPD, "Eight" }, { 9, SPD, "Nine" }, { 10, SPD, "Ten" }, { 10, SPD, "Jack" }, { 10, SPD, "Queen" }, { 10, SPD, "King" },
		{ 1, HRT, "Ace" }, { 2, HRT, "Two" }, { 3, HRT, "Three" }, { 4, HRT, "Four" }, { 5, HRT, "Five" }, { 6, HRT, "Six" }, { 7, HRT, "Seven" },
		{ 8, HRT, "Eight" }, { 9, HRT, "Nine" }, { 10, HRT, "Ten" }, { 10, HRT, "Jack" }, { 10, HRT, "Queen" }, { 10, HRT, "King" },
		{ 1, DIA, "Ace" }, { 2, DIA, "Two" }, { 3, DIA, "Three" }, { 4, DIA, "Four" }, { 5, DIA, "Five" }, { 6, DIA, "Six" }, { 7, DIA, "Seven" },
		{ 8, DIA, "Eight" }, { 9, DIA, "Nine" }, { 10, DIA, "Ten" }, { 10, DIA, "Jack" }, { 10, DIA, "Queen" }, { 10, DIA, "King" },
		{ 1, CLB, "Ace" }, { 2, CLB, "Two" }, { 3, CLB, "Three" }, { 4, CLB, "Four" }, { 5, CLB, "Five" }, { 6, CLB, "Six" }, { 7, CLB, "Sever" },
		{ 8, CLB, "Eight" }, { 9, CLB, "Nine" }, { 10, CLB, "Ten" }, { 10, CLB, "Jack" }, { 10, CLB, "Queen" }, { 10, CLB, "King" },
		{ 1, SPD, "Ace" }, { 2, SPD, "Two" }, { 3, SPD, "Three" }, { 4, SPD, "Four" }, { 5, SPD, "Five" }, { 6, SPD, "Six" }, { 7, SPD, "Seven" },
		{ 8, SPD, "Eight" }, { 9, SPD, "Nine" }, { 10, SPD, "Ten" }, { 10, SPD, "Jack" }, { 10, SPD, "Queen" }, { 10, SPD, "King" },
		{ 1, HRT, "Ace" }, { 2, HRT, "Two" }, { 3, HRT, "Three" }, { 4, HRT, "Four" }, { 5, HRT, "Five" }, { 6, HRT, "Six" }, { 7, HRT, "Seven" },
		{ 8, HRT, "Eight" }, { 9, HRT, "Nine" }, { 10, HRT, "Ten" }, { 10, HRT, "Jack" }, { 10, HRT, "Queen" }, { 10, HRT, "King" },
		{ 1, DIA, "Ace" }, { 2, DIA, "Two" }, { 3, DIA, "Three" }, { 4, DIA, "Four" }, { 5, DIA, "Five" }, { 6, DIA, "Six" }, { 7, DIA, "Seven" },
		{ 8, DIA, "Eight" }, { 9, DIA, "Nine" }, { 10, DIA, "Ten" }, { 10, DIA, "Jack" }, { 10, DIA, "Queen" }, { 10, DIA, "King" },
		{ 1, CLB, "Ace" }, { 2, CLB, "Two" }, { 3, CLB, "Three" }, { 4, CLB, "Four" }, { 5, CLB, "Five" }, { 6, CLB, "Six" }, { 7, CLB, "Sever" },
		{ 8, CLB, "Eight" }, { 9, CLB, "Nine" }, { 10, CLB, "Ten" }, { 10, CLB, "Jack" }, { 10, CLB, "Queen" }, { 10, CLB, "King" }
	};
}

void dealCards(vector<card> &deck, vector<card> &players_hand, vector<card> &house_hand) {
	playerBlackjack = false;
	houseBlackjack = false;		//Clear all the important flags
	insurance = false;
	playerDouble = false;
	playerSurrender = false;
	firstHand = true;
	int c; 								//random number used to deal a card from the deck
	c = rand() % deck.size(); 			//generate random card number
	players_hand.push_back(deck[c]);	//deal first card to players hand
	deck.erase(deck.begin() + c); 		//remove dealt card from the deck

	c = rand() % deck.size(); 		//generate another random card number
	house_hand.push_back(deck[c]); 	//deal the first card to the house
	deck.erase(deck.begin() + c); 	//remove dealt card from the deck

	c = rand() % deck.size(); 			//generate another random card number
	players_hand.push_back(deck[c]); 	//deal the second card to the player
	deck.erase(deck.begin() + c); 		//remove dealt card from the deck

	c = rand() % deck.size(); 		//generate another random card number
	house_hand.push_back(deck[c]); 	//deal the second card to the house
	deck.erase(deck.begin() + c); 	//remove dealt card from the deck

	// Checks whether player and/or dealer has Blackjack
	if (((players_hand[0].r == 10) && (players_hand[1].r == 1)) || ((players_hand[0].r == 1) && (players_hand[1].r == 10))) {
		playerBlackjack = true;
	}

	if ((house_hand[0].r == 10) && (house_hand[1].r == 1)) {
		houseBlackjack = true;
	}
}

void hit(vector<card> &players_hand) {
	firstHand = false;	// It's no longer the first hand
	int c; 								//random number used to deal a card from the deck
	c = rand() % deck.size(); 			//generate random card number
	players_hand.push_back(deck[c]); 	//deal first card to players hand
	deck.erase(deck.begin() + c); 		//remove dealt card from the deck
}

void doubleDown(vector<card> &players_hand) {
	int c; 								//random number used to deal a card from the deck
	c = rand() % deck.size(); 			//generate random card number
	players_hand.push_back(deck[c]); 	//deal first card to players hand
	deck.erase(deck.begin() + c); 		//remove dealt card from the deck
	playerDouble = true;
}

void surrender(vector<card> &players_hand) {
	playerSurrender = true; //Sets the player to surrender
}

// A function that plays one full game of Blackjack
void play() {
	// Initialize the player and house sums
	unsigned int player_sum = 0;
	unsigned int house_sum = 0;

	shuffle(deck); // shuffle the deck and clear the hands
	dealCards(deck, players_hand, house_hand); //deal the first set of cards

	// Check for blackjack before play starts
	if (!playerBlackjack && !houseBlackjack) {
		playerLogic(players_hand); // Hit/stay for the player
		houseLogic(house_hand); // Hit/stay for the house

		// Calculate the player and house sums
		for(unsigned int i = 0; i < players_hand.size(); i++) {
			player_sum += players_hand[i].r;
		}
		for(unsigned int i = 0; i < players_hand.size(); i++) {
			if (players_hand[i].r == 1) {
				if ((player_sum + 10) < 22) {
					player_sum += 10;
				}
			}
		}

		for(unsigned int i = 0; i < house_hand.size(); i++) {
			house_sum += house_hand[i].r;
		}
		for(unsigned int i = 0; i < house_hand.size(); i++) {
			if (house_hand[i].r == 1) {
				if ((house_sum + 10) < 22) {
					house_sum += 10;
				}
			}
		}

		cout << "The player had: ";
		cout << player_sum << endl;

		cout << "The dealer had: ";
		cout << house_sum << endl;

		// Win or Lose conditions
		if (playerSurrender) {
			cout << "YOU SURRENDERED" << endl;
			players_bank -= players_bet/2;
		} else if(playerDouble){
			if (player_sum > 21) {
				cout << "YOU LOST THE DOUBLE" << endl;
				players_bank -= (2 * players_bet);
			} else if (house_sum > 21) {
				cout << "YOU WINN THE DOUBLE" << endl;
				players_bank += (2 * players_bet);
			} else if (player_sum < house_sum) {
				cout << "YOU LOST THE DOUBLE" << endl;
				players_bank -= (2 * players_bet);
			} else if (player_sum > house_sum) {
				cout << "YOU WINN THE DOUBLE" << endl;
				players_bank += (2 * players_bet);
			} else {
				cout << "PUSH" << endl;
			}
		} else {
			if (player_sum > 21) {
				cout << "YOU LOSE!!" << endl;
				players_bank -= players_bet;
			} else if (house_sum > 21) {
				cout << "YOU WINN!!!!" << endl;
				players_bank += players_bet;
			} else if (player_sum < house_sum) {
				cout << "YOU LOSE!!" << endl;
				players_bank -= players_bet;
			} else if (player_sum > house_sum) {
				cout << "YOU WINN!!!!" << endl;
				players_bank += players_bet;
			} else {
				cout << "PUSH" << endl;
			}
		}
	} else if (playerBlackjack && !houseBlackjack) {
		// Calculate the player and house sum to print
		for(unsigned int i = 0; i < players_hand.size(); i++) {
			player_sum += players_hand[i].r;
		}
		for(unsigned int i = 0; i < players_hand.size(); i++) {
			if (players_hand[i].r == 1) {
				if ((player_sum + 10) < 22) {
					player_sum += 10;
				}
			}
		}

		for(unsigned int i = 0; i < house_hand.size(); i++) {
			house_sum += house_hand[i].r;
		}
		for(unsigned int i = 0; i < house_hand.size(); i++) {
			if (house_hand[i].r == 1) {
				if ((house_sum + 10) < 22) {
					house_sum += 10;
				}
			}
		}

		cout << "The player had: ";
		cout << player_sum << endl;

		cout << "The dealer had: ";
		cout << house_sum << endl;

		cout << "YOU GOT BLACKJACK" << endl;
		players_bank += (players_bet + 5);
	} else if (!playerBlackjack && houseBlackjack) {
		// Calculate player and house sum to print for debugging
		for(unsigned int i = 0; i < players_hand.size(); i++) {
			player_sum += players_hand[i].r;
		}
		for(unsigned int i = 0; i < players_hand.size(); i++) {
			if (players_hand[i].r == 1) {
				if ((player_sum + 10) < 22) {
					player_sum += 10;
				}
			}
		}

		for(unsigned int i = 0; i < house_hand.size(); i++) {
			house_sum += house_hand[i].r;
		}
		for(unsigned int i = 0; i < house_hand.size(); i++) {
			if (house_hand[i].r == 1) {
				if ((house_sum + 10) < 22) {
					house_sum += 10;
				}
			}
		}

		cout << "The player had: ";
		cout << player_sum << endl;

		cout << "The dealer had: ";
		cout << house_sum << endl;

		cout << "THE DEALER HAD BLACKJACK" << endl;
		players_bank -= players_bet;
	} else {
		// Calculate the player and house sum for printing
		for(unsigned int i = 0; i < players_hand.size(); i++) {
			player_sum += players_hand[i].r;
		}
		for(unsigned int i = 0; i < players_hand.size(); i++) {
			if (players_hand[i].r == 1) {
				if ((player_sum + 10) < 22) {
					player_sum += 10;
				}
			}
		}

		for(unsigned int i = 0; i < house_hand.size(); i++) {
			house_sum += house_hand[i].r;
		}
		for(unsigned int i = 0; i < house_hand.size(); i++) {
			if (house_hand[i].r == 1) {
				if ((house_sum + 10) < 22) {
					house_sum += 10;
				}
			}
		}

		cout << "The player had: ";
		cout << player_sum << endl;

		cout << "The dealer had: ";
		cout << house_sum << endl;

		cout << "BOTH HAD BLACKJACK" << endl;
	}
}

// The logic the "player" will follow, based on BJ basic strategy
// and contains every action a player should take based on the house's "face
// card"
void playerLogic(vector<card> &players_hand) {
	unsigned int player_sum = 0;
	unsigned int house_sum = 0;		// Clear flag and house sum
	playerSoftHand = false;

	// Calculating the players current hand for use to decide what action to take
	for(unsigned int i = 0; i < players_hand.size(); i++) {
		player_sum += players_hand[i].r;
	}
	for(unsigned int i = 0; i < players_hand.size(); i++) {
		if (players_hand[i].r == 1) {
			if ((player_sum + 10) < 22) {
				player_sum += 10;
				playerSoftHand = true;
			}
		}
	}

	house_sum = house_hand[0].r; 	// Calculating the house upcard which is just the
									// first card of the house's hand

	// Every player action based on the dealer upcard
	if (!playerSoftHand) {
		switch(player_sum) {
		case 2:
			hit(players_hand);
			playerLogic(players_hand);
			break;
		case 3:
			hit(players_hand);
			playerLogic(players_hand);
			break;
		case 4:
			hit(players_hand);
			playerLogic(players_hand);
			break;
		case 5:
			hit(players_hand);
			playerLogic(players_hand);
			break;
		case 6:
			hit(players_hand);
			playerLogic(players_hand);
			break;
		case 7:
			hit(players_hand);
			playerLogic(players_hand);
			break;
		case 8:
			hit(players_hand);
			playerLogic(players_hand);
			break;
		case 9:
			switch(house_sum) {
			case 1:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 2:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 3:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 4:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 5:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 6:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 7:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 8:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 9:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 10:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			default:
				break;
			}
			break;
		case 10:
			switch(house_sum) {
			case 1:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 2:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 3:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 4:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 5:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 6:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 7:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 8:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 9:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 10:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			default:
				break;
			}
			break;
		case 11:
			switch(house_sum) {
			case 1:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 2:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 3:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 4:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 5:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 6:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 7:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 8:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 9:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 10:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			default:
				break;
			}
			break;
		case 12:
			switch(house_sum) {
			case 1:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 2:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 3:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				break;
			case 7:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 8:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 9:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 10:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			default:
				break;
			}
			break;
		case 13:
			switch(house_sum) {
			case 1:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				break;
			case 7:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 8:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 9:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 10:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			default:
				break;
			}
			break;
		case 14:
			switch(house_sum) {
			case 1:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				break;
			case 7:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 8:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 9:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 10:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			default:
				break;
			}
			break;
		case 15:
			switch(house_sum) {
			case 1:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				break;
			case 7:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 8:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 9:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 10:
				if (firstHand) {
					surrender(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			default:
				break;
			}
			break;
		case 16:
			switch(house_sum) {
			case 1:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				break;
			case 7:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 8:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 9:
				if (firstHand) {
					surrender(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 10:
				if (firstHand) {
					surrender(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			default:
				break;
			}
			break;
		case 17:
			break;
		case 18:
			break;
		case 19:
			break;
		case 20:
			break;
		case 21:
			break;
		default:
			break;
		}
	} else {
		switch(player_sum) {
		case 12:
			switch(house_sum) {
			case 1:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 2:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 3:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 4:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 5:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 6:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 7:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 8:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 9:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 10:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			default:
				break;
			}
			break;
		case 13:
			switch(house_sum) {
			case 1:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 2:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 3:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 4:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 5:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 6:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 7:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 8:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 9:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 10:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			default:
				break;
			}
			break;
		case 14:
			switch(house_sum) {
			case 1:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 2:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 3:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 4:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 5:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 6:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 7:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 8:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 9:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 10:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			default:
				break;
			}
			break;
		case 15:
			switch(house_sum) {
			case 1:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 2:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 3:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 4:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 5:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 6:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 7:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 8:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 9:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 10:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			default:
				break;
			}
			break;
		case 16:
			switch(house_sum) {
			case 1:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 2:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 3:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 4:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 5:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 6:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 7:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 8:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 9:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 10:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			default:
				break;
			}
			break;
		case 17:
			switch(house_sum) {
			case 1:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 2:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 3:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 4:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 5:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 6:
				if(firstHand) {
					doubleDown(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 7:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 8:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 9:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 10:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			default:
				break;
			}
			break;
		case 18:
			switch(house_sum) {
			case 1:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 2:
				break;
			case 3:
				if(firstHand) {
					doubleDown(players_hand);
				}
				break;
			case 4:
				if(firstHand) {
					doubleDown(players_hand);
				}
				break;
			case 5:
				if(firstHand) {
					doubleDown(players_hand);
				}
				break;
			case 6:
				if(firstHand) {
					doubleDown(players_hand);
				}
				break;
			case 7:
				break;
			case 8:
				break;
			case 9:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 10:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			default:
				break;
			}
			break;
		case 19:
			break;
		case 20:
			break;
		case 21:
			break;
		default:
			break;
		}
	}
}

// The logic the house must follow, this logic is based only on the dealers hand
// and occurs after the player has hit/stand based on the dealers first card
void houseLogic(vector<card> &house_hand) {
	unsigned int house_sum = 0;

	// Calculating the dealers hand in order to decide what to do
	for(unsigned int i = 0; i < house_hand.size(); i++) {
		house_sum += house_hand[i].r;
	}
	for(unsigned int i = 0; i < house_hand.size(); i++) {
		if (house_hand[i].r == 1) {
			if ((house_sum + 10) < 22) {
				house_sum += 10;
			}
		}
	}

	// The combinations of all possible dealer hands and what the dealer must to
	// for each hand
	switch(house_sum) {
	case 2:
		hit(house_hand);
		houseLogic(house_hand);
		break;
	case 3:
		hit(house_hand);
		houseLogic(house_hand);
		break;
	case 4:
		hit(house_hand);
		houseLogic(house_hand);
		break;
	case 5:
		hit(house_hand);
		houseLogic(house_hand);;
		break;
	case 6:
		hit(house_hand);
		houseLogic(house_hand);
		break;
	case 7:
		hit(house_hand);
		houseLogic(house_hand);
		break;
	case 8:
		hit(house_hand);
		houseLogic(house_hand);
		break;
	case 9:
		hit(house_hand);
		houseLogic(house_hand);
		break;
	case 10:
		hit(house_hand);
		houseLogic(house_hand);
		break;
	case 11:
		hit(house_hand);
		houseLogic(house_hand);
		break;
	case 12:
		hit(house_hand);
		houseLogic(house_hand);
		break;
	case 13:
		hit(house_hand);
		houseLogic(house_hand);
		break;
	case 14:
		hit(house_hand);
		houseLogic(house_hand);
		break;
	case 15:
		hit(house_hand);
		houseLogic(house_hand);
		break;
	case 16:
		hit(house_hand);
		houseLogic(house_hand);
		break;
	case 17:
		break;
	case 18:
		break;
	case 19:
		break;
	case 20:
		break;
	case 21:
		break;
	default:
		break;
	}
}
