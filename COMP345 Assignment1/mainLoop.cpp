#include <iostream>
#include <string>
#include "mainLoop.h"
using namespace std;

int MainLoop::numPlayers = 0;
int MainLoop::turnsRemaining = 0;

//Constructor that currently can only handle a 2 player game
MainLoop::MainLoop(vector<Player*> players, Deck* deck, Map* map) {
	this->players = players;
	this->deck = deck;
	this->map = map;
	this->numPlayers = players.size();
	if (numPlayers == 2)
		this->turnsRemaining = 26;
	else {
		cout << "\n*Error*, must be two players.";
		exit(0);
	}
}


//Displays whose turn is it and how many game turns remain
std::ostream& operator<<(std::ostream& strm, const MainLoop& m) {
	Player* turn = m.whoseTurn();
	return strm << "It is " << turn->getName() << "'s turn. " << m.turnsRemaining << " turns remaining in the game.";
}


//Check whose turn it is
Player* MainLoop::whoseTurn() const{
	//int turn = turnsRemaining%(this->players.size());
	return this->players.at(turnsRemaining%numPlayers);
}


//Display Map&Hand
void MainLoop::showBoard() {
	cout << "**************************\n"; 
	//map->printMap();
	cout << *deck;
	
}

//Determine the action from a card and have the player execute the action
void MainLoop::takeAction(Player* p, Card *c) {
	
	bool andAction = false;
	bool orAction = false;
	int bonus = 0;
	int num = 0;
	int len = 0;
	int first = 0;
	int second = 0;
	int third = 0;
	int fourth = 0;
	string split = "";
	string ret = "";
	int looper = 1;
	int tempAction = c->getAction();
	if (tempAction < 0) {
		orAction = true;
		tempAction*=-1;
	}
	
	for (int i = tempAction; i > 0; i = i/10) {
		len++;
	}
	
	if (len == 4) {
		if (!orAction) {
			andAction = true;
			looper = 2;
		}
		
		first = tempAction/1000;
		second = (tempAction/100)%10;
		third = (tempAction/10)%10;
		fourth = tempAction%10;
	}
	else if (len == 2) {
		first = tempAction/10;
		second = tempAction%10;
	}
	else {
		first = tempAction;
	}
	
	for (int i = 0; i < looper; i++) {

		if (orAction) {
			int choice = 0;
			cout << *c << endl;
			cout << "Which action would you like to take? (0 or 1) ->";
			cin >> choice;
			if (choice == 1) {
				first = third;
				second = fourth;
			}
		}
		
		if (i == 1) {
			int choice = 0;
			cout << *c <<endl;
			cout << "Would you like to take the second action? (0 for No, 1 for Yes) ->";
			cin >> choice;
			if (choice == 1) {
				first = third;
				second = fourth;
			}
		}

		switch (first) {
			case 1:
				bonus = p->checkPlacementBonus();
				num = second + bonus;
				cout << "Placing " << second << " armies with a bonus of " << bonus << " for a total of " << num;
				p->PlaceNewArmies(num);
				break;
			case 2:
				bonus = p->checkMovementBonus();
				num = second + bonus;
				cout << "Moving " << second << " armies with a bonus of " << bonus << " for a total of " << num << ".\nMoving over water costs " << p->checkFlying() << " per Army.";
				p->MoveArmies(num);
				break;
			case 3:
				cout << "Destroying an army...";
				p->DestroyArmy();
				break;
			case 4:
				cout << "Building a city...";
				p->BuildCity();
				break;
			default:
				ret+= "Invalid...";
		}
	}
	
}

//A single turn
void MainLoop::singleTurn(Player *p) {
	
	//Display the Map & Hand
	showBoard();
	
	//Player chooses the card they want from the hand
	int cardChoice;
	cout << p->getName() <<"... Enter the card selection # that you would like to choose: ";
	cin >> cardChoice;
	
	//Find and display the card;
	Card* theCard = deck->getHand()->getCardByIndex(cardChoice);
	cout << *theCard << endl;
	
	//Player takes the card from the hand, hand is updated
	p->exchange(deck, theCard);
	
	//Player takes the action of the Card
	takeAction(p, theCard);
	
	//Show the player all their cards, end turn
	cout << "\n\n**************************\nHere are all your cards: \n";
	for (Card* c : p->getPlayerCards()) {
		cout << *c;
	}
	cout << "\n**************************\n...Turn Over...\n\n";
}

void MainLoop::playGame() {
	
	//Welcome message
	cout << "*~*~*~*~*~*~*~*~*~*~*~*~*~*~*\n*~*~Welcome to Eight-Minute Empire: Legends!~*~*\n";
	
	//Fixed # of turns per game to manage the mainloop
	while (turnsRemaining > 0) {
		
		//determine whose turn
		Player* thePlayer = whoseTurn();
		
		//Run turn for the player
		singleTurn(thePlayer);
		
		//Decrement turns
		turnsRemaining--;
	}
	
	cout << "\nCard limit reached! Time to tally the score...\n";
	
}

