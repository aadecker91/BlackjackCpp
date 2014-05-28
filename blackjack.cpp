// enum.cpp by Bill Weinman <http://bw.org/>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
using namespace std;

const string SPD = "Spade";
const string HRT = "Heart";
const string DIA = "Diamond";
const string CLB = "Club";


struct card {
	unsigned int r;
	string s;
};

// Variables used
const int players_bet = 10;
static int players_bank = 10000;
vector<card> players_hand;
vector<card> house_hand;
vector<card> deck = {
			{ 1, SPD }, { 2, SPD }, { 3, SPD }, { 4, SPD }, { 5, SPD }, { 6, SPD }, { 7, SPD },
			{ 8, SPD }, { 9, SPD }, { 10, SPD }, { 10, SPD }, { 10, SPD }, { 10, SPD },
			{ 1, HRT }, { 2, HRT }, { 3, HRT }, { 4, HRT }, { 5, HRT }, { 6, HRT }, { 7, HRT },
			{ 8, HRT }, { 9, HRT }, { 10, HRT }, { 10, HRT }, { 10, HRT }, { 10, HRT },
			{ 1, DIA }, { 2, DIA }, { 3, DIA }, { 4, DIA }, { 5, DIA }, { 6, DIA }, { 7, DIA },
			{ 8, DIA }, { 9, DIA }, { 10, DIA }, { 10, DIA }, { 10, DIA }, { 10, DIA },
			{ 1, CLB }, { 2, CLB }, { 3, CLB }, { 4, CLB }, { 5, CLB }, { 6, CLB }, { 7, CLB },
			{ 8, CLB }, { 9, CLB }, { 10, CLB }, { 10, CLB }, { 10, CLB }, { 10, CLB }
	};

//function declarations
void shuffle(vector<card> &deck);
void dealCards(vector<card> &deck, vector<card> &players_hand, vector<card> &house_hand);
int hit(unsigned int &player_sum, vector<card> &players_hand);
void play();

int main( int argc, char ** argv ) {
	play();
	cout << players_bank << endl;
	cout << "The remaining deck is: " << endl;
	for(unsigned i = 0; i < deck.size(); i++){
		cout << deck[i].r << "  " << deck[i].s << endl;
	}
	cout << "Your hand is: " << endl;
	for(unsigned i = 0; i < players_hand.size(); i++){
		cout << players_hand[i].r << "  " << players_hand[i].s << endl;
	}
	cout << "The houses hand is:" << endl;
	for(unsigned i = 0; i < house_hand.size(); i++){
		cout << house_hand[i].r << "  " << house_hand[i].s << endl;
	}
	return 0;
}
// Resets the deck
/*void shuffle(vector<card> &deck) {
	deck = {
			{ ACE, SPD }, { 2, SPD }, { 3, SPD }, { 4, SPD }, { 5, SPD }, { 6, SPD }, { 7, SPD },
			{ 8, SPD }, { 9, SPD }, { 10, SPD }, { JACK, SPD }, { QUEEN, SPD }, { KING, SPD },
			{ 1, HRT }, { 2, HRT }, { 3, HRT }, { 4, HRT }, { 5, HRT }, { 6, HRT }, { 7, HRT },
			{ 8, HRT }, { 9, HRT }, { 10, HRT }, { JACK, HRT }, { QUEEN, HRT }, { KING, HRT },
			{ 1, DIA }, { 2, DIA }, { 3, DIA }, { 4, DIA }, { 5, DIA }, { 6, DIA }, { 7, DIA },
			{ 8, DIA }, { 9, DIA }, { 10, DIA }, { JACK, DIA }, { QUEEN, DIA }, { KING, DIA },
			{ 1, CLB }, { 2, CLB }, { 3, CLB }, { 4, CLB }, { 5, CLB }, { 6, CLB }, { 7, CLB },
			{ 8, CLB }, { 9, CLB }, { 10, CLB }, { JACK, CLB }, { QUEEN, CLB }, { KING, CLB }
	};
}
*/

void dealCards(vector<card> &deck, vector<card> &players_hand, vector<card> &house_hand) {
	int c; //random number used to deal a card from the deck
	srand(time(NULL));
	c = rand() % deck.size(); //generate random card number
	players_hand.push_back(deck[c]); //deal first card to players hand
	deck.erase(deck.begin() + c); //remove dealt card from the deck

	srand(time(NULL));
	c = rand() % deck.size();
	players_hand.push_back(deck[c]);
	deck.erase(deck.begin() + c);

	srand(time(NULL));
	c = rand() % deck.size();
	house_hand.push_back(deck[c]);
	deck.erase(deck.begin() + c);

	srand(time(NULL));
	c = rand() % deck.size();
	house_hand.push_back(deck[c]);
	deck.erase(deck.begin() + c);

//	for(unsigned i = 0; i < deck.size(); i++){
//		cout << deck[i].r << "  " << deck[i].s << endl;
//	}
//
//	for(unsigned i = 0; i < players_hand.size(); i++){
//		cout << players_hand[i].r << "  " << players_hand[i].s << endl;
//	}
//
//	for(unsigned i = 0; i < house_hand.size(); i++){
//		cout << house_hand[i].r << "  " << house_hand[i].s << endl;
//	}
}

int hit(unsigned int &player_sum, vector<card> &players_hand) {
	int c; //random number used to deal a card from the deck
	srand(time(NULL));
	c = rand() % deck.size(); //generate random card number
	players_hand.push_back(deck[c]); //deal first card to players hand
	deck.erase(deck.begin() + c); //remove dealt card from the deck

	for(unsigned i = 0; i < players_hand.size(); i++) {
		player_sum += players_hand[i].r;
	}
	return player_sum;
}

void play() {
	unsigned int player_sum = 0;
	unsigned int house_sum = 0;
	dealCards(deck, players_hand, house_hand);

	for(unsigned int i = 0; i < players_hand.size(); i++) {
		player_sum += players_hand[i].r;
	}

	switch(player_sum) {
		case 2:
			hit(player_sum, players_hand);
			break;
		case 3:
			hit(player_sum, players_hand);
			break;
		case 4:
			hit(player_sum, players_hand);
			break;
		case 5:
			hit(player_sum, players_hand);
			break;
		case 6:
			hit(player_sum, players_hand);
			break;
		case 7:
			hit(player_sum, players_hand);
			break;
		case 8:
			hit(player_sum, players_hand);
			break;
		case 9:
			hit(player_sum, players_hand);
			break;
		case 10:
			hit(player_sum, players_hand);
			break;
		case 11:
			hit(player_sum, players_hand);
			break;
		case 12:
			hit(player_sum, players_hand);
			break;
		case 13:
			hit(player_sum, players_hand);
			break;
		case 14:
			hit(player_sum, players_hand);
			break;
		case 15:
			hit(player_sum, players_hand);
			break;
		case 16:
			hit(player_sum, players_hand);
			break;
		case 17:
			hit(player_sum, players_hand);
			break;
		case 18:
			hit(player_sum, players_hand);
			break;
		case 19:
			hit(player_sum, players_hand);
			break;
		case 20:
			hit(player_sum, players_hand);
			break;
		default:
			break;
	}

	for(unsigned int i = 0; i < house_hand.size(); i++) {
		house_sum += house_hand[i].r;
	}

	switch(house_sum) {
		case 2:
			hit(house_sum, house_hand);
			break;
		case 3:
			hit(house_sum, house_hand);
			break;
		case 4:
			hit(house_sum, house_hand);
			break;
		case 5:
			hit(house_sum, house_hand);
			break;
		case 6:
			hit(house_sum, house_hand);
			break;
		case 7:
			hit(house_sum, house_hand);
			break;
		case 8:
			hit(house_sum, house_hand);
			break;
		case 9:
			hit(house_sum, house_hand);
			break;
		case 10:
			hit(house_sum, house_hand);
			break;
		case 11:
			hit(house_sum, house_hand);
			break;
		case 12:
			hit(house_sum, house_hand);
			break;
		case 13:
			hit(house_sum, house_hand);
			break;
		case 14:
			hit(house_sum, house_hand);
			break;
		case 15:
			hit(house_sum, house_hand);
			break;
		case 16:
			hit(house_sum, house_hand);
			break;
		case 17:
			break;
		case 18:
			break;
		case 19:
			break;
		default:
			break;
	}

	if (house_sum > player_sum) {
		cout << "YOU LOSE!!" << endl;
		players_bank -= players_bet;
	} else if (player_sum > house_sum) {
		cout << "YOU WINN!!!!" << endl;
		players_bank += players_bet;
	} else {
		cout << "PUSH" << endl;
	}
}

