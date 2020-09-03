#include "game.h"
#include "player.h"
#include "textDisplay.h"
#include "graphicDisplay.h"
#include "link.h"
#include <sstream>

using namespace std; 


bool Game::isGameOver(){
	if ((player1->dataDownloaded == 4) || 
	    (player1->virusDownloaded == 4) || 
	    (player2->dataDownloaded == 4) || 
	    (player2->virusDownloaded == 4)){
		return true; 
	} 
	return false; 
}

Owner Game::whoWon(){
	if(player1->dataDownloaded == 4) {
		return Owner::playerOne;
	}
	else if(player1->virusDownloaded == 4){
		return Owner::playerTwo;
	}
	else if(player2->dataDownloaded == 4){
		return Owner::playerTwo;
	}
	else if (player2->virusDownloaded == 4){
		return Owner::playerOne;
	}
}

void Game::printAbilities(){
	if (currentPlayer == Owner::playerOne){
		player1->printAbilities(); 
	} else {
		player2->printAbilities(); 
	}
}

void Game::useAbility(int num, string arguments){
	char ab; 

	// ************* Modified Code Begins **************************************** // 
	
	if (currentPlayer == Owner::playerOne){

		if ((!player1->abilityUsedThisTurn) && (!player1->abilitiesUsed[num-1])){
			ab = player1->abilitySequence[num - 1]; 
			player1->abilityUsedThisTurn = true; 

		 } else if (player1->abilityUsedThisTurn){
			 cout << "You have used your ability already for this turn!" << endl; 
			 return; 

		 } else if (player1->abilitiesUsed[num - 1]){
			 cout << "This ability is not available anymore" << endl;  
			 return; 
		 }
	}

	else {
	       if ((!player2->abilityUsedThisTurn) && (!player2->abilitiesUsed[num-1])){	
			ab = player2->abilitySequence[num-1];
			player2->abilityUsedThisTurn = true; 

		} else if (player2->abilityUsedThisTurn){
			cout << "You have used your ability already for this turn!" << endl; 
			return; 

		} else if (player2->abilitiesUsed[num - 1]){
			cout << "This ability is not available anymore" << endl; 
			return; 
		}
	} 

	// ****************** Modified Code Begins ****************************************** // 

        if (ab == 'F') {
		istringstream iss(arguments);
		int r, c;
		iss >> r >> c;
		int pos = r * 8 + c;
		if (!pInfo[pos].isItOccupied && !pInfo[pos].isFW) {
			if (currentPlayer == Owner::playerOne) {
				pInfo[pos] = {false, true, Owner::playerOne, Owner::playerOne};
				player1->useAbility(num, arguments);
			} else {
				pInfo[pos] = {false, true, Owner::playerTwo, Owner::playerTwo};
				player2->useAbility(num, arguments);
			}
		} 
	} else {
		if (currentPlayer == Owner::playerOne)
			player1->useAbility(num, arguments); 
		else player2->useAbility(num, arguments); 
	}
}

Game::Game(string OneLinks, string OneAbilities, string TwoLinks, string TwoAbilities, bool graphicalInterface): graphicsEnabled {graphicalInterface} {

	player1 = new Player{Owner::playerOne, OneLinks, OneAbilities}; 
	player2 = new Player{Owner::playerTwo, TwoLinks, TwoAbilities}; 
	td = new TextDisplay{};  

        player1->attach(td);
	player2->attach(td);

	for(auto it : player1->theLinks) {
		player1->attach(it);
		player2->attach(it);
	}

	for (auto it: player2->theLinks) {
		player2->attach(it);
		player1->attach(it);
	}

	if (graphicsEnabled){
		gd = new GraphicDisplay{};
		player1->attach(gd);
		player2->attach(gd);
        } 

	for (int i = 0; i < 8; i ++){

		player1->theLinks[i]->attach(td);
		player1->theLinks[i]->attach(this); 

		if (graphicsEnabled){
			player1->theLinks[i]->attach(gd);
		}

		player1->theLinks[i]->attach(player1); 
		player1->theLinks[i]->attach(player2); 

		player1->theLinks[i]->notifyObservers(); 

		player2->theLinks[i]->attach(td);
		player2->theLinks[i]->attach(this); 
		
		if (graphicsEnabled){
	        	player2->theLinks[i]->attach(gd);	
		}

		player2->theLinks[i]->attach(player1); 
		player2->theLinks[i]->attach(player2); 

		player2->theLinks[i]->notifyObservers(); 

	} 
	
	
	for (int i = 0; i < 8; i ++){
		for (int j = 0; j < 8; j ++){
			player1->theLinks[i]->attach(player2->theLinks[j]); 
			player2->theLinks[i]->attach(player1->theLinks[j]); 
		}
	}

	pInfo = new pieceInfo[64]; 

	for (int i = 0; i < 64; i ++){
		pInfo[i] = {false, false, Owner::playerOne, Owner::playerOne}; 
	}

	for (int i = 0; i < 8; i ++){
		for (int j = 0; j < 8; j ++){

			if (i == 0){
				if ((j == 3) || (j == 4)){
					pInfo[11] = {true, false, Owner::playerOne}; 
					pInfo[12] = {true, false, Owner::playerOne}; 
				}

				else {
					pInfo[j] = {true, false, Owner::playerOne}; 
					pInfo[j] = {true, false, Owner::playerOne}; 
				}
			}


			if (i == 7){
				if ((j == 3) || (j == 4)){
					pInfo[51] = {true, false, Owner::playerTwo}; 
					pInfo[52] = {true, false, Owner::playerTwo}; 
				} else {
					pInfo[8*i + j] = {true, false, Owner::playerTwo}; 
					pInfo[8*i + j] = {true, false, Owner::playerTwo}; 
				}
			}

		}
	}

	currentPlayer = Owner::playerOne;
	td->currentPlayer = currentPlayer; 

}

Game::~Game(){

	delete[] pInfo; 
	if (graphicsEnabled){
		delete gd; 
	}

	delete td; 
	delete player1; 
	delete player2; 	
}

void Game::move(char name, Direction d){

	bool inValidMove = false; 
	bool initiatingBattle = false;
	bool boost = false;
	bool downloadByFW = false;

	if (currentPlayer == Owner::playerOne)
		boost = player1->is_boost(name);
	else
		boost = player2->is_boost(name);

	int numspace = boost? 2 : 1;
	
        Posi currentPos;
        Posi nextPos;
    
        // ************************** Modified Code Begins Here **********************

	if (currentPlayer == Owner::playerOne){
		if (player1->theLinks[name - 97]->isFrozen){
			cout << "Link is frozen for this turn. Move another link." << endl; 
			return; 
		}
	} else {
		if (player2->theLinks[name - 65]->isFrozen){
			cout << "Link is frozen for this turn. Move another link." << endl; 
			return; 
		}
	} 

	// ************************ Modified Code Ends Here ******************************	
	

	if (currentPlayer == Owner::playerOne){

		if ((name < 97) || (name > 104)){
			inValidMove = true;  
		} else {
			currentPos = player1->getLinkPos(name); 
			nextPos = currentPos; 
			if (d == Direction::Up){
				nextPos.x = currentPos.x - numspace; 
			} else if (d == Direction::Down){
				nextPos.x = currentPos.x + numspace; 
			} else if (d == Direction::Left){
				nextPos.y = currentPos.y - numspace; 
			}else {
				nextPos.y = currentPos.y + numspace; 
			}

			if ((nextPos.x == 0) && (nextPos.y == 3 || nextPos.y == 4)){
				cout << "Sever Ports" << endl; 
				inValidMove = true; 
			}	 
		}
	} 
		
	if (currentPlayer == Owner::playerTwo){

		if ((name < 65) || (name > 72)){
			inValidMove = true;  
		} else {
			currentPos = player2->getLinkPos(name); 
			nextPos = currentPos; 
			if (d == Direction::Up){
				nextPos.x = currentPos.x - numspace; 
			} else if (d == Direction::Down){
				nextPos.x = currentPos.x + numspace; 
			} else if (d == Direction::Left){
				nextPos.y = currentPos.y - numspace; 
			} else {
				nextPos.y = currentPos.y + numspace; 
			}

			if ((nextPos.x == 7) && (nextPos.y == 3 || nextPos.y == 4)){
				cout << "Server Ports" << endl; 
				inValidMove = true;
			}	 
		}
	}

	if (currentPlayer == Owner::playerOne){
		if (nextPos.x < 0 || nextPos.y > 7 || nextPos.y < 0){
			//cout << "Out of bounds" << endl; 
			//cout << "Current Position " << currentPos.x << "  " << currentPos.y << endl; 
			//cout << "Next Position: " << nextPos.x << "   " << nextPos.y << endl; 
			inValidMove = true; 
		}
	} else {
		if (nextPos.x > 7 || nextPos.y > 7 || nextPos.y < 0){
			inValidMove = true; 
		}
	}

	if (!inValidMove && (nextPos.x >= 0) && (nextPos.x <= 7) && pInfo[nextPos.x*8 + nextPos.y].isFW == true) {

		if (currentPlayer != pInfo[nextPos.x*8 + nextPos.y].whoseFW) {
                        if (currentPlayer == Owner::playerOne) {
					for (int i = 0; i < 8; i++) {
						if (player1->theLinks[i]->name == name) {
							LinkType t = player1->theLinks[i]->getInfo().type;
							if (t == LinkType::virus) downloadByFW = true;
							break;
						}
					}
				} else {
					for (int i = 0; i < 8; i++) {
						if (player2->theLinks[i]->name == name) {
							LinkType t = player2->theLinks[i]->getInfo().type;
							if (t == LinkType::virus) downloadByFW = true;
							break;
						}
					}
				}
		}
	}
				 
  
	if (downloadByFW == false  && !inValidMove && (nextPos.x >= 0) && (nextPos.x <= 7) && pInfo[nextPos.x*8 + nextPos.y].isItOccupied){
		//cout << "Position is occupied at row: " << nextPos.x << " and column: " << nextPos.y <<  "accessed by: " << name << endl; 

		if (pInfo[nextPos.x*8 + nextPos.y].currentPlayer == currentPlayer){
			//cout << "Team player is present here" << endl;

			       inValidMove = true;
		     
		} else {
			initiatingBattle = true;
		}

	} 
	

	if (!inValidMove){
		if (currentPlayer == Owner::playerOne){
                        gd->currentPlayer = Owner::playerTwo;
			player1->move(name, d, initiatingBattle, downloadByFW); 
		} else {
                        gd->currentPlayer = Owner::playerOne;
			player2->move(name, d, initiatingBattle, downloadByFW);
		}
	
		if (currentPlayer == Owner::playerOne){
			currentPlayer = Owner::playerTwo; 
			td->currentPlayer = currentPlayer;
		} else {
			currentPlayer = Owner::playerOne; 
			td->currentPlayer = currentPlayer;	
		}
	} else {
		cout << "Invalid move" << endl; 
	}
} 

bool Game::isInactivePlayer(char name){ 
	for (int i = 0; i < inactivePlayers.length(); i ++){
		if (inactivePlayers[i] == name){
			return true; 
		}
	}
	return false; 
}

void Game::notify(Subject <Info, State> &whoFrom){
	State linkState = whoFrom.getState(); 
        Info linkInfo = whoFrom.getInfo(); 
	bool boost = linkInfo.boosted;
	int increment = boost ? 2 : 1;

	if (linkState.state == StateType::NewPiece){
		//cout << "New piece" << endl; 
	} 

	else if (linkState.state == StateType::JustMoved){

		int currentPos = linkInfo.row*8 + linkInfo.col; 
		int prevPos;	

		if (linkInfo.ownedBy == Owner::playerOne){

			if (linkState.direction == Direction::Up){
				prevPos = (linkInfo.row+increment)*8 + linkInfo.col; 
			} else if (linkState.direction == Direction::Down){
				prevPos = (linkInfo.row-increment)*8 + linkInfo.col; 
			} else if (linkState.direction == Direction::Left){
				prevPos = linkInfo.row*8 + (linkInfo.col+increment);  
			} else {
				prevPos = linkInfo.row*8 + (linkInfo.col-increment);
			}

			/*
			if (prevPos == 35){
				cout << linkInfo.name  << " moving away from 35 " << endl; 
			}
			*/

			pInfo[prevPos].isItOccupied = false; 
		} 

		if (linkInfo.ownedBy == Owner::playerTwo){

			if (linkState.direction == Direction::Up){
				prevPos = (linkInfo.row+increment)*8 + linkInfo.col; 
			} else if (linkState.direction == Direction::Down){
				prevPos = (linkInfo.row-increment)*8 + linkInfo.col; 
			} else if (linkState.direction == Direction::Left){
				prevPos = linkInfo.row*8 + (linkInfo.col+increment); 
			} else {
				prevPos = linkInfo.row*8 + (linkInfo.col-increment); 
			} 

			pInfo[prevPos].isItOccupied = false; 

			/*
			if (prevPos == 35){
				cout << linkInfo.name << " moving away from 35" << endl; 
			}
			*/
		}  

		pInfo[currentPos].isItOccupied = true;
		pInfo[currentPos].currentPlayer = linkInfo.ownedBy; 
	}	

	else if (linkState.state == StateType::DownloadedByPlayerOne){
		 
		int currentPos = linkInfo.row*8 + linkInfo.col; 

		// Modified Code Begins
		inactivePlayers += linkInfo.name; 
		// Modified Code Ends 

		if (currentPos == 3 || currentPos == 4){
			int prevPos; 

			if (linkState.direction == Direction::Up){
				prevPos = (linkInfo.row+increment)*8 + linkInfo.col; 
			} else if (linkState.direction == Direction::Left){
				prevPos = linkInfo.row*8 + linkInfo.col+increment; 
			} else if (linkState.direction == Direction::Right){
				prevPos = linkInfo.row*8 + linkInfo.col-increment; 
			}

			pInfo[prevPos].isItOccupied = false; 
				
		} else {
			//cout << "Current pos with row: " << linkInfo.row << " and col:  " << linkInfo.col; 
			//cout << " is being updated to false" << endl; 
			pInfo[currentPos].isItOccupied = false; 
		}
	} 

	else if (linkState.state == StateType::DownloadedByPlayerTwo){
		int currentPos = linkInfo.row*8 + linkInfo.col; 
		int prevPos; 

		//Modified Code Begins
		inactivePlayers += linkInfo.name; 
		// Modified Code Ends 

		if (currentPos == 59 || currentPos == 60){

			if (linkState.direction == Direction::Down){
				prevPos = (linkInfo.row-increment)*8 + linkInfo.col; 
			} else if (linkState.direction == Direction::Left){
				prevPos = linkInfo.row*8 + linkInfo.col+increment; 
			} else if (linkState.direction == Direction::Right){
				prevPos = linkInfo.row*8 + linkInfo.col-increment; 
			} 

			pInfo[prevPos].isItOccupied = false; 

		} else {

			//cout << "Current pos with row: " << linkInfo.row << " and col: " << linkInfo.col; 
			//cout << "is being updated to false " << endl; 

			pInfo[currentPos].isItOccupied = false; 
		}
	}

	  else if (linkState.state == StateType::LostBattle){
		
		int currentPos = linkInfo.row*8 + linkInfo.col; 
		pInfo[currentPos].isItOccupied = true; 
		pInfo[currentPos].currentPlayer = whoFrom.getOpponentInfo().ownedBy;  

	}  else if (linkState.state == StateType::InitiateBattle){
		
		int prevPos; 

		if (linkState.direction == Direction::Down){
			prevPos = (linkInfo.row-increment)*8 + linkInfo.col; 
		} else if (linkState.direction == Direction::Up){
			prevPos = (linkInfo.row+increment)*8 + linkInfo.col; 
		} else if (linkState.direction == Direction::Left){
			prevPos = linkInfo.row*8 + linkInfo.col+increment; 
		} else {
			prevPos = linkInfo.row*8 + linkInfo.col-increment; 
		}

		pInfo[prevPos].isItOccupied = false; 
	}
}

ostream &operator<<(ostream &out, const Game &g){
	cout << * g.td; 
}
