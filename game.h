#ifndef GAME_HEADER
#define GAME_HEADER
#include <iostream>
#include <string>
#include <vector>
#include "state.h"
#include "observer.h"
#include "info.h"
#include "posi.h"

struct pieceInfo{
	bool isItOccupied;
        bool isFW;
	Owner currentPlayer;
	Owner whoseFW;
}; 

class Player; 
class TextDisplay; 
class GraphicDisplay; 
class Game: Observer<Info, State>{
	
	Player *player1; 
	Player *player2; 
	TextDisplay *td;
	GraphicDisplay *gd; 

	// ******* Modified Code Begins ******** //
	std::string inactivePlayers = ""; 
	// ******* Modified Code Ends ********* // 

	bool graphicsEnabled; 

	public:
	
	// *********** Modified Code Begins ****** // 
	bool isInactivePlayer(char name); 
	// ********** Modified Code Ends ******* // 

	Owner currentPlayer; 

	struct pieceInfo* pInfo;  

       	Game(std::string player1Links, 
	     std::string player1Abilities, 
	     std::string player2Links, 
	     std::string player2Abilities, 
	     bool graphicalInterface=false);

	~Game(); 

	void move(char name, Direction d); 
	void useAbility(int num, std::string arguments); 

	void notify(Subject <Info, State> &whoFrom) override;

	bool isGameOver(); 	
	void printAbilities();
	Owner whoWon(); 
	

	friend std::ostream &operator<<(std::ostream &out, const Game &g); 
}; 

#endif
