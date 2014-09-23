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
#include <random>
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
bool playerDouble;			// Flag used to indicate a player doubling down
bool playerDouble2;			//  on a perticular hand
bool playerDouble3;
bool playerDouble4;
bool playerSurrender;		// Used to indicate a player surrendering
bool firstHand;				// Used to indicate a a freshly dealt hand
vector<card> players_hand; 			// The player's hand
bool playerSoftHand;	//indicates the player has a soft hand
vector<card> players_hand2; // The rest of these hands are used only when a player splits his hand
bool playerSoftHand2;
vector<card> players_hand3;
bool playerSoftHand3;
vector<card> players_hand4;
bool playerSoftHand4;
vector<card> house_hand; 	// The houses hand
bool houseSoftHand;		//indicates the house has a soft hand
vector<card> deck;		// The deck used to deal cards

//function declarations
void shuffle(vector<card> &deck); //used to reshuffle the deck
void dealCards(vector<card> &deck, vector<card> &players_hand, vector<card> &house_hand); // Deals the cards to the player and the house
void hit(vector<card> &players_hand); //add's one card to a hand (either the player's or the house's
void doubleDown(vector<card> &players_hand); // Double down: Player gets one more card and they double their bet
void split(vector<card> &players_hand); 	// Player splits his hand, gets one more card for each hand, and plays the remainder of each hand
void surrender(vector<card> &players_hand);  //Surrender the hand to the house, lose half of original bet
void playerLogic(vector<card> &players_hand); // The logic the player will follow
void houseLogic(vector<card> &house_hand);
void play(); // Plays a single perfect game for the player

int main( int argc, char ** argv ) {
	//srand(time(NULL));		// Seeds the rand generator used to deal cards from the deck
	string command = "";	// User input
	// when the user input is anything but q, deal a game of blackjack and print
	//  the results
	//while(command != "q") {
	for (int i = 0; i < 1000; i++) {
		for (int j = 0; j < 1000; j++) {
			play();
		}
	}
		// Prints the users bankroll after the last game of BJ
		cout << players_bank << endl;

		// Prints the players hand(s) after the game for the user to see
		cout << "Your hand is: " << endl;
		for(unsigned i = 0; i < players_hand.size(); i++){
			cout << players_hand[i].r << " of " << players_hand[i].s << endl;
		}
		if (!players_hand2.empty()) {
			cout << "Your second hand is: " << endl;
			for(unsigned i = 0; i < players_hand2.size(); i++){
				cout << players_hand2[i].r << " of " << players_hand2[i].s << endl;
			}
		}
		if (!players_hand3.empty()) {
			cout << "Your third hand is: " << endl;
			for(unsigned i = 0; i < players_hand3.size(); i++){
				cout << players_hand3[i].r << " of " << players_hand3[i].s << endl;
			}
		}
		if (!players_hand4.empty()) {
			cout << "Your fourth hand is: " << endl;
			for(unsigned i = 0; i < players_hand4.size(); i++){
				cout << players_hand4[i].r << " of " << players_hand4[i].s << endl;
			}
		}
		// Prints the house hand for the user
		cout << "The houses hand is:" << endl;
		for(unsigned i = 0; i < house_hand.size(); i++){
			cout << house_hand[i].r << " of " << house_hand[i].s << endl;
		}
		// Get next command from user
		cin >> command;
	//}
	return 0;
}
// Resets the deck and the house/player hands
void shuffle(vector<card> &deck) {
	// Erase both the player's and the house's hand(s)
	players_hand.erase(players_hand.begin(), players_hand.end());
	players_hand2.erase(players_hand2.begin(), players_hand2.end());
	players_hand3.erase(players_hand3.begin(), players_hand3.end());
	players_hand4.erase(players_hand4.begin(), players_hand4.end());
	house_hand.erase(house_hand.begin(), house_hand.end());
	// Reshuffle the six decks
	deck = {
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
	houseBlackjack = false;		//Clear all the important flags before the new hand is dealt
	insurance = false;
	playerDouble = false;
	playerDouble2 = false;
	playerDouble3 = false;
	playerDouble4 = false;
	playerSurrender = false;
	firstHand = true;
	static mt19937 mt(time(NULL));
	uniform_int_distribution<int> dist( 0, 51) ;
	players_hand.push_back(deck[dist(mt)]);	//deal first card to players hand
	//deck.erase(deck.begin() + dist(mt)); 		//remove dealt card from the deck

	house_hand.push_back(deck[dist(mt)]); 	//deal the first card to the house
	//deck.erase(deck.begin() + dist(mt)); 	//remove dealt card from the deck

	players_hand.push_back(deck[dist(mt)]);	//deal first card to players hand
	//deck.erase(deck.begin() + dist(mt)); 		//remove dealt card from the deck

	house_hand.push_back(deck[dist(mt)]); 	//deal the first card to the house
	//deck.erase(deck.begin() + dist(mt)); 	//remove dealt card from the deck

	// Checks whether player has Blackjack and sets the appropriate flag
	if (((players_hand[0].r == 10) && (players_hand[1].r == 1)) || ((players_hand[0].r == 1) && (players_hand[1].r == 10))) {
		playerBlackjack = true;
	}
	// This is the dealers "peek," if the dealer has a ten showing he will peek for blackjack
	// the insurance option is not included because you're never supposed to take insurance
	// and the deck is shuffled after every hand so counting cards can't affct your
	// decision to take insurance
	if ((house_hand[0].r == 10) && (house_hand[1].r == 1)) {
		houseBlackjack = true;
	}
}

void hit(vector<card> &players_hand) {
	firstHand = false;					// It's no longer the first hand
	static mt19937 mt(time(NULL));
	uniform_int_distribution<int> dist( 0, 51) ;
	players_hand.push_back(deck[dist(mt)]); 	//deal first card to players hand
	//deck.erase(deck.begin() + c); 		//remove dealt card from the deck
}

void doubleDown(vector<card> &players_hand) {
	static mt19937 mt(time(NULL));
	uniform_int_distribution<int> dist( 0, 51) ;
	players_hand.push_back(deck[dist(mt)]); 	//deal first card to players hand
	//deck.erase(deck.begin() + c); 		//remove dealt card from the deck
	// Set the flag for the deck the player doubled on
	if (&players_hand == &players_hand2) {
		playerDouble2 = true;
	} else if (&players_hand == &players_hand3) {
		playerDouble3 = true;
	} else if (&players_hand == &players_hand4) {
		playerDouble4 = true;
	} else {
		playerDouble = true;
	}

}

void surrender(vector<card> &players_hand) {
	playerSurrender = true; //Sets the flag for player to surrender
			// the player can only surrender on the original hand that is dealt
}

void split(vector<card> &players_hand) {
	card temp;							// used to store one of the split cards
	// Checks which hands are empty to ensure 1. the player cannot split to more than four hands and
	// 2. The player's split will not overwrite/add to a previous split
	if (players_hand2.empty()) {
		temp = players_hand.back();		// stores one of the players cards
		players_hand.pop_back();		// removes that card
		players_hand2.push_back(temp);	// pushes that card to another hand
		hit(players_hand);				// hits both hands
		hit(players_hand2);
		firstHand = true;				// set this flag to ensure that the player can double after a split
		playerLogic(players_hand);		// follow player logic for the first new hand
		firstHand = true;				// set this flag to ensure that the player can double after a split
		playerLogic(players_hand2);		// follow player logic for the second new hand
	} else if (players_hand3.empty()) {
		temp = players_hand.back();
		players_hand.pop_back();
		players_hand3.push_back(temp);
		hit(players_hand);
		hit(players_hand3);
		firstHand = true;
		playerLogic(players_hand);
		firstHand = true;
		playerLogic(players_hand3);		// The code for the other 2 possible cases
	} else if (players_hand4.empty()) {	// this gives the player the opportunity to split a hand
		temp = players_hand.back();		// up to four times
		players_hand.pop_back();
		players_hand4.push_back(temp);
		hit(players_hand);
		hit(players_hand4);
		firstHand = true;
		playerLogic(players_hand);
		firstHand = true;
		playerLogic(players_hand4);
	} else {
	}
}

// A function that plays one full game of Blackjack
void play() {
	// Initialize the player and house sums to be zero
	unsigned int player_sum = 0;
	unsigned int house_sum = 0;

	shuffle(deck); // shuffle the deck and clear the hands
	dealCards(deck, players_hand, house_hand); //deal the first set of cards

	// Check for blackjack before play starts
	if (!playerBlackjack && !houseBlackjack) {
		playerLogic(players_hand); // Hit/stay/double/split logic for the player
		houseLogic(house_hand); // Hit/stay logic for the house

		// This is for a game where the player did not split his hand
		if (players_hand2.empty()) {
			// Calculate the player sum
			for(unsigned int i = 0; i < players_hand.size(); i++) {
				player_sum += players_hand[i].r;
			}
			// Calculates whether an ace will be treated as a one or eleven
			for(unsigned int i = 0; i < players_hand.size(); i++) {
				if (players_hand[i].r == 1) {
					if ((player_sum + 10) < 22) {
						player_sum += 10;
					}
				}
			}

			// Calculate the house sum
			for(unsigned int i = 0; i < house_hand.size(); i++) {
				house_sum += house_hand[i].r;
			}
			// Calculates whether an ace will be treated as a one or eleven
			for(unsigned int i = 0; i < house_hand.size(); i++) {
				if (house_hand[i].r == 1) {
					if ((house_sum + 10) < 22) {
						house_sum += 10;
					}
				}
			}

			// prints the players sum
			cout << "The player had: ";
			cout << player_sum << endl;

			// prints the house sum
			cout << "The dealer had: ";
			cout << house_sum << endl;

			// Win or Lose conditions
			if (playerSurrender) {
				cout << "YOU SURRENDERED" << endl;
				players_bank -= players_bet/2;		// player loses half his bet if he surrenders
			} else if(playerDouble){		// players gains/loses doubles his bet if he doubles
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
		} else {		// the win/lose conditions when the player has multiple hands
			player_sum = 0;
			house_sum = 0;
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
			if(playerDouble){
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

			// the player's second hand sum and win/lose conditions
			player_sum = 0;
			house_sum = 0;
			// Calculate the player and house sums
			for(unsigned int i = 0; i < players_hand2.size(); i++) {
				player_sum += players_hand2[i].r;
			}
			for(unsigned int i = 0; i < players_hand2.size(); i++) {
				if (players_hand2[i].r == 1) {
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
			if(playerDouble2){
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
			// checks if the player had split to three hands and then checks
			// the win/lose conditions and adds/subtracts money from the bank
			// based on those conditions
			if (!players_hand3.empty()) {
				player_sum = 0;
				house_sum = 0;
				// Calculate the player and house sums
				for(unsigned int i = 0; i < players_hand3.size(); i++) {
					player_sum += players_hand3[i].r;
				}
				for(unsigned int i = 0; i < players_hand3.size(); i++) {
					if (players_hand3[i].r == 1) {
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
				if(playerDouble3){
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
			}
			// checks if the player split to four hands and then checks the win/loss
			// conditions for the fourth hand and acts accordingly
			if (!players_hand4.empty()) {
				player_sum = 0;
				house_sum = 0;
				// Calculate the player and house sums
				for(unsigned int i = 0; i < players_hand4.size(); i++) {
					player_sum += players_hand4[i].r;
				}
				for(unsigned int i = 0; i < players_hand4.size(); i++) {
					if (players_hand4[i].r == 1) {
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
				if(playerDouble4){
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
			}
		}
	} else if (playerBlackjack && !houseBlackjack) {
		// what happens when the player has blackjack

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
		// what happens when the house has blackjack

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
		// what happens when both the player and the house have blackjack

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
	// used to distinguish when the player has a soft hand
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

	// The following is logic for how the player should play when confronted with
	// any dealer upcard, it is a series of embedded case/if statements
	// the "hard" player hands are first
	if (!playerSoftHand) {
		switch(player_sum) {
		case 4:
			switch(house_sum) {
			case 1:
				hit(players_hand);
				playerLogic(players_hand);
				break;
			case 2:
				if (players_hand4.empty()) {
					split(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 3:
				if (players_hand4.empty()) {
					split(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 4:
				if (players_hand4.empty()) {
					split(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 5:
				if (players_hand4.empty()) {
					split(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 6:
				if (players_hand4.empty()) {
					split(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
				break;
			case 7:
				if (players_hand4.empty()) {
					split(players_hand);
				} else {
					hit(players_hand);
					playerLogic(players_hand);
				}
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
		case 5:
			hit(players_hand);
			playerLogic(players_hand);
			break;
		case 6:
			if ((players_hand[0].r == 3) && (players_hand[1].r == 3)) {
				switch(house_sum) {
				case 1:
					hit(players_hand);
					playerLogic(players_hand);
					break;
				case 2:
					if (players_hand4.empty()) {
						split(players_hand);
					} else {
						hit(players_hand);
						playerLogic(players_hand);
					}
					break;
				case 3:
					if (players_hand4.empty()) {
						split(players_hand);
					} else {
						hit(players_hand);
						playerLogic(players_hand);
					}
					break;
				case 4:
					if (players_hand4.empty()) {
						split(players_hand);
					} else {
						hit(players_hand);
						playerLogic(players_hand);
					}
					break;
				case 5:
					if (players_hand4.empty()) {
						split(players_hand);
					} else {
						hit(players_hand);
						playerLogic(players_hand);
					}
					break;
				case 6:
					if (players_hand4.empty()) {
						split(players_hand);
					} else {
						hit(players_hand);
						playerLogic(players_hand);
					}
					break;
				case 7:
					if (players_hand4.empty()) {
						split(players_hand);
					} else {
						hit(players_hand);
						playerLogic(players_hand);
					}
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
			if ((players_hand[0].r == 4) && (players_hand[1].r == 4)) {
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
					if (players_hand4.empty()) {
						split(players_hand);
					} else {
						hit(players_hand);
						playerLogic(players_hand);
					}
					break;
				case 6:
					if (players_hand4.empty()) {
						split(players_hand);
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
			} else {
				hit(players_hand);
				playerLogic(players_hand);
			}
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
			if ((players_hand[0].r == 6) && (players_hand[1].r == 6)) {
				switch(house_sum) {
				case 1:
					hit(players_hand);
					playerLogic(players_hand);
					break;
				case 2:
					if (players_hand4.empty()) {
						split(players_hand);
					} else {
						hit(players_hand);
						playerLogic(players_hand);
					}
					break;
				case 3:
					if (players_hand4.empty()) {
						split(players_hand);
					} else {
						hit(players_hand);
						playerLogic(players_hand);
					}
					break;
				case 4:
					if (players_hand4.empty()) {
						split(players_hand);
					}
					break;
				case 5:
					if (players_hand4.empty()) {
						split(players_hand);
					}
					break;
				case 6:
					if (players_hand4.empty()) {
						split(players_hand);
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
			} else {
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
			if ((players_hand[0].r == 7) && (players_hand[1].r == 7)) {
				switch(house_sum) {
				case 1:
					hit(players_hand);
					playerLogic(players_hand);
					break;
				case 2:
					if (players_hand4.empty()) {
						split(players_hand);
					}
					break;
				case 3:
					if (players_hand4.empty()) {
						split(players_hand);
					}
					break;
				case 4:
					if (players_hand4.empty()) {
						split(players_hand);
					}
					break;
				case 5:
					if (players_hand4.empty()) {
						split(players_hand);
					}
					break;
				case 6:
					if (players_hand4.empty()) {
						split(players_hand);
					}
					break;
				case 7:
					if (players_hand4.empty()) {
						split(players_hand);
					} else {
						hit(players_hand);
						playerLogic(players_hand);
					}
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
			} else {
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
					// check if the players split hand is empty to ensure
					// the player is not surrendering a split hand
					if (players_hand2.empty()) {
						surrender(players_hand);
					} else {
						hit(players_hand);
						playerLogic(players_hand);
					}
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
			if ((players_hand[0].r == 8) && (players_hand[1].r == 8)) {
				switch(house_sum) {
				case 1:
					if (players_hand4.empty()) {
						split(players_hand);
					} else {
						hit(players_hand);
						playerLogic(players_hand);
					}
					break;
				case 2:
					if (players_hand4.empty()) {
						split(players_hand);
					}
					break;
				case 3:
					if (players_hand4.empty()) {
						split(players_hand);
					}
					break;
				case 4:
					if (players_hand4.empty()) {
						split(players_hand);
					}
					break;
				case 5:
					if (players_hand4.empty()) {
						split(players_hand);
					}
					break;
				case 6:
					if (players_hand4.empty()) {
						split(players_hand);
					}
					break;
				case 7:
					if (players_hand4.empty()) {
						split(players_hand);
					} else {
						hit(players_hand);
						playerLogic(players_hand);
					}
					break;
				case 8:
					if (players_hand4.empty()) {
						split(players_hand);
					} else {
						hit(players_hand);
						playerLogic(players_hand);
					}
					break;
				case 9:
					if (players_hand4.empty()) {
						split(players_hand);
					} else {
						hit(players_hand);
						playerLogic(players_hand);
					}
					break;
				case 10:
					if (players_hand4.empty()) {
						split(players_hand);
					} else {
						hit(players_hand);
						playerLogic(players_hand);
					}
					break;
				default:
					break;
				}
			} else {
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
						// check if the players split hand is empty to ensure
						// the player is not surrendering a split hand
						if (players_hand2.empty()) {
							surrender(players_hand);
						} else {
							hit(players_hand);
							playerLogic(players_hand);
						}
					} else {
						hit(players_hand);
						playerLogic(players_hand);
					}
					break;
				case 10:
					if (firstHand) {
						// check if the players split hand is empty to ensure
						// the player is not surrendering a split hand
						if (players_hand2.empty()) {
							surrender(players_hand);
						} else {
							hit(players_hand);
							playerLogic(players_hand);
						}
					} else {
						hit(players_hand);
						playerLogic(players_hand);
					}
					break;
				default:
					break;
				}
			}
			break;
		case 17:
			break;
		case 18:
			if ((players_hand[0].r == 9) && (players_hand[1].r == 9)) {
				switch(house_sum) {
				case 1:
					break;
				case 2:
					if (players_hand4.empty()) {
						split(players_hand);
					}
					break;
				case 3:
					if (players_hand4.empty()) {
						split(players_hand);
					}
					break;
				case 4:
					if (players_hand4.empty()) {
						split(players_hand);
					}
					break;
				case 5:
					if (players_hand4.empty()) {
						split(players_hand);
					}
					break;
				case 6:
					if (players_hand4.empty()) {
						split(players_hand);
					}
					break;
				case 7:
					break;
				case 8:
					if (players_hand4.empty()) {
						split(players_hand);
					}
					break;
				case 9:
					if (players_hand4.empty()) {
						split(players_hand);
					}
					break;
				case 10:
					break;
				default:
					break;
				}
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
	} else {
		// Soft hand player logic
		switch(player_sum) {
		case 12:
			if (players_hand4.empty()) {
				split(players_hand);
			} else {
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
	// Determine whether an ace will act as a one or eleven
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
