#include "textDisplay.h" 
#include "subject.h"
#include <iostream>
#include <map>
#include <utility>
using namespace std;

TextDisplay::TextDisplay(){
	for (int i = 0; i < 8; i ++){
		theBoard.push_back(vector<char>()); 
		for (int j = 0; j < 8; j ++){
			theBoard[i].push_back('.'); 
		}
	}

	dDownloadedP1 = vDownloadedP1 = dDownloadedP2 = vDownloadedP2 = 0; 

	theBoard[0][3] = 'S'; 
	theBoard[0][4] = 'S'; 

	theBoard[7][3] = 'S'; 
	theBoard[7][4] = 'S'; 
} 

void TextDisplay::printPiecesPlayerOne() {

	for (const auto &myPair: linksPlayerOne){
		if (myPair.first == 'e'){
			cout << endl; 
		}

		cout << myPair.first <<  " : "; 

		if ((myPair.second.hidden) && (currentPlayer == Owner::playerTwo)){
			cout << "? "; 
		} else {
			cout << myPair.second.type << myPair.second.strength;	
		}

		cout << "   "; 
	}

	cout << endl; 
} 

void TextDisplay::printPiecesPlayerTwo() {
	for (const auto &myPair: linksPlayerTwo){
		if (myPair.first == 'E'){
			cout << endl; 
		}
		cout << myPair.first << " : "; 

		if ((myPair.second.hidden) && (currentPlayer == Owner::playerOne)){
			cout << "? "; 
		} else {
			cout << myPair.second.type << myPair.second.strength; 
		} 

		cout << "   "; 
	}

	cout << endl; 
}

void TextDisplay::notify(Subject <Info, State> &whoFrom){

	State linkState = whoFrom.getState(); 
	Info linkInfo = whoFrom.getInfo(); 

	if (linkState.state == StateType::NewPiece){

		positionMapping.insert(pair<char, Pos>(linkInfo.name, {linkInfo.row, linkInfo.col})); 
		updateBoard(linkInfo.name); 

		char type = 'V'; 

		if (linkInfo.type == LinkType::data){
			type = 'D'; 
		}

		if (linkInfo.ownedBy == Owner::playerOne){
			linksPlayerOne.insert(pair<char, LinkInfo>(linkInfo.name, {type, linkInfo.strength, true})); 
		} else {
			linksPlayerTwo.insert(pair<char, LinkInfo>(linkInfo.name, {type, linkInfo.strength, true})); 
		}

	} 

	else if (linkState.state == StateType::JustMoved){
		//cout << "A piece just moved" << endl; 

		int old_pos_x = positionMapping[linkInfo.name].x; 
		int old_pos_y = positionMapping[linkInfo.name].y;

		int size = firewallpair.size();
		int i = 0;
		Pos p = {old_pos_x, old_pos_y};
		for(; i < size; i++) {
			if (firewallpair[i].first == p) {
				theBoard[old_pos_x][old_pos_y] = firewallpair[i].second;
				break;
			}
		}

		if (i == size) theBoard[old_pos_x][old_pos_y] = '.'; 

		positionMapping[linkInfo.name] = {linkInfo.row, linkInfo.col}; 
		//cout << " x column is " << positionMapping[linkInfo.name].x << endl; 
		//cout << " y columns is " << positionMapping[linkInfo.name].y << endl; 
		updateBoard(linkInfo.name); 
	} 

	else if (linkState.state == StateType::InitiateBattle){

		//cout << "Initiate Battle - Text Display" << endl; 

		int old_pos_x = positionMapping[linkInfo.name].x; 
		int old_pos_y = positionMapping[linkInfo.name].y; 

		int size = firewallpair.size();
		int i = 0;
		Pos p = {old_pos_x, old_pos_y};
		for(; i < size; i++) {
			if (firewallpair[i].first == p) {
				theBoard[old_pos_x][old_pos_y] = firewallpair[i].second;
				break;
			}
		}
		if (i == size) theBoard[old_pos_x][old_pos_y] = '.';

		positionMapping[linkInfo.name] = {linkInfo.row, linkInfo.col};


	} else if ((linkState.state == StateType::LostBattle) || (linkState.state == StateType::WonBattle)){

		Info Opponent = whoFrom.getOpponentInfo(); 

		if (linkInfo.ownedBy == Owner::playerOne){
			linksPlayerOne[linkInfo.name].hidden = false; 
			linksPlayerTwo[Opponent.name].hidden = false; 
		} else {
			linksPlayerTwo[linkInfo.name].hidden = false; 
			linksPlayerOne[Opponent.name].hidden = false; 
		} 

		if (linkState.state == StateType::WonBattle){

			//cout << "Won Battle Update" << endl; 

			if (linkInfo.ownedBy == Owner::playerOne){
				if (Opponent.type == LinkType::virus){
					vDownloadedP1 ++; 
				} else {
					dDownloadedP1 ++; 
				}
			} else {
				if (Opponent.type == LinkType::virus){
					vDownloadedP2 ++; 
				} else {
					dDownloadedP2 ++; 
				}
			}

		} else {

			//cout << "Lost Battle Update" << endl; 

			if (Opponent.ownedBy == Owner::playerOne){
				if (linkInfo.type == LinkType::virus){
					vDownloadedP1 ++; 
				} else {
					dDownloadedP1 ++; 
				}
			} else {
				if (linkInfo.type == LinkType::virus){
					vDownloadedP2 ++; 
				} else {
					dDownloadedP2 ++; 
				}
			}

			updateBoard(Opponent.name); 

		}
	}

	else if (linkState.state == StateType::DownloadedByPlayerOne){

		if (linkInfo.type == LinkType::data){
			dDownloadedP1 ++; 
		} else {
			vDownloadedP1 ++; 
		} 

		int old_pos_x = positionMapping[linkInfo.name].x;
		int old_pos_y = positionMapping[linkInfo.name].y;

		int size = firewallpair.size();
		int i = 0;
		Pos p = {old_pos_x, old_pos_y};
		for(; i < size; i++) {
			if (firewallpair[i].first == p) {
				theBoard[old_pos_x][old_pos_y] = firewallpair[i].second;
				break;
			}
		}
		if (i == size) theBoard[old_pos_x][old_pos_y] = '.';

		if (linkInfo.ownedBy == Owner::playerTwo){
			linksPlayerTwo[linkInfo.name].hidden = false;  
		}
		
	        // ******************* Modified Code ******************
		
		else {
			linksPlayerOne[linkInfo.name].hidden = false; 
		} 

		// *********************** Modified Code ****************
	} 

	else if (linkState.state == StateType::DownloadedByPlayerTwo){

		//cout << "Downloaded player two " << endl; 

		if (linkInfo.type == LinkType::data){
			dDownloadedP2 ++; 
		} else {
			vDownloadedP2 ++; 
		}

		int old_pos_x = positionMapping[linkInfo.name].x; 
		int old_pos_y = positionMapping[linkInfo.name].y; 

		int size = firewallpair.size();
		int i = 0;
		Pos p = {old_pos_x, old_pos_y};
		for(; i < size; i++) {
			if (firewallpair[i].first == p) {
				theBoard[old_pos_x][old_pos_y] = firewallpair[i].second;
				break;
			}
		}

		if (i == size) theBoard[old_pos_x][old_pos_y] = '.'; 

		//cout << "Text Display is being updated at row: " << old_pos_x << "  and col:  " << old_pos_y; 
		//cout << endl; 

		
		if (linkInfo.ownedBy == Owner::playerOne){
			linksPlayerOne[linkInfo.name].hidden = false;  
		}
		
	 	// ******************* Modified Code ****************** // 	
		else {
			linksPlayerTwo[linkInfo.name].hidden = false; 
		}

		// ****************** Modified Code ******************** // 
	}

	else if (linkState.state == StateType::Boost) {
		if (linkInfo.ownedBy == Owner::playerOne) numAbilitiesRemP1 -= 1;
		else numAbilitiesRemP2 -= 1;
	}

        // ****************** Modified Code Begins Here ***********************

	else if (linkState.state == StateType::QuickFreeze) {
		if (linkInfo.ownedBy == Owner::playerOne) numAbilitiesRemP1 -= 1; 
		else numAbilitiesRemP2 -= 1; 
	}

	// ******************** Modified Code Ends Here ***************************

	else if (linkState.state == StateType::Firewall) {
		fPos wpos = linkState.FWpos;
		int r = wpos.x;
		int c = wpos.y;
		Pos p = {r, c};
		if (linkInfo.ownedBy == Owner::playerOne) {
			theBoard[r][c] = 'm';
			firewallpair.push_back( make_pair(p, 'm') );
			numAbilitiesRemP1 -= 1;

		} else {
			theBoard[r][c] = 'w';
			firewallpair.push_back(make_pair(p, 'w'));
			numAbilitiesRemP2 -= 1;
		}
	}


	else if (linkState.state == StateType::Scan) {
		if(linkInfo.ownedBy == Owner::playerOne) {
			linksPlayerTwo[linkInfo.name].hidden = false;
			numAbilitiesRemP1 -= 1;
		} else {
			linksPlayerOne[linkInfo.name].hidden = false;
			numAbilitiesRemP2 -= 1;
		}
	}
	else if (linkState.state == StateType::Polarize) {
		if((linkInfo.ownedBy == Owner::playerOne) && ((linkInfo.name >= 'a') && (linkInfo.name <= 'h'))) {
			if (linksPlayerOne[linkInfo.name].type == 'D') {
				linksPlayerOne[linkInfo.name].type ='V';
			} else {
				linksPlayerOne[linkInfo.name].type = 'D';
			}

			numAbilitiesRemP1 -= 1;
		}
		else if((linkInfo.ownedBy == Owner::playerOne) && ((linkInfo.name >= 'A') && (linkInfo.name <= 'H'))) {
			if (linksPlayerTwo[linkInfo.name].type == 'D') {
				linksPlayerTwo[linkInfo.name].type ='V';
			} else {
				linksPlayerTwo[linkInfo.name].type = 'D';
			}

			numAbilitiesRemP1 -= 1;
		}

		else if((linkInfo.ownedBy == Owner::playerTwo) && ((linkInfo.name >= 'A') && (linkInfo.name <= 'H'))) {
			if (linksPlayerTwo[linkInfo.name].type == 'D') {
				linksPlayerTwo[linkInfo.name].type ='V';
			} else {
				linksPlayerTwo[linkInfo.name].type = 'D';
			}

			numAbilitiesRemP2 -= 1;
		}

		else if((linkInfo.ownedBy == Owner::playerTwo) && ((linkInfo.name >= 'a') && (linkInfo.name <= 'h'))) {
			if (linksPlayerOne[linkInfo.name].type == 'D') {
				linksPlayerOne[linkInfo.name].type ='V';
			} else {
				linksPlayerOne[linkInfo.name].type = 'D';
			}

			numAbilitiesRemP2 -= 1;
		}
	}

    else if (linkState.state == StateType::Download){
		if(linkInfo.ownedBy == Owner::playerOne) {
			numAbilitiesRemP1 -= 1;
		} else {
			numAbilitiesRemP2 -= 1;
		}
	}

	else if (linkState.state == StateType:: Intensify){
		if(linkInfo.ownedBy == Owner::playerOne){
			linksPlayerOne[linkInfo.name].strength += 1;
		}
		if (linkInfo.ownedBy == Owner::playerTwo) {
			linksPlayerTwo[linkInfo.name].strength +=1;
		}
	}
	
	else if (linkState.state == StateType::Antidote) {
		if(linkInfo.ownedBy == Owner:: playerOne){
			if(vDownloadedP1 == 0){
				vDownloadedP1 = 0;
			} else {
			vDownloadedP1--;
			}
		}

		if(linkInfo.ownedBy == Owner:: playerTwo){
			if(vDownloadedP2 == 0){
				vDownloadedP2 = 0;
			} else {
			vDownloadedP2--;
			}
		}
	}

} 

void TextDisplay::updateBoard(char name){
	//cout << "Error here" << endl; 
	int pos_x = positionMapping[name].x; 
	//cout << "Extraction" << endl; 
	int pos_y = positionMapping[name].y; 
	//cout << "Update link with char name: " << name << " stored at row: " << pos_x << " and col: " << pos_y; 
	theBoard[pos_x][pos_y] = name;

	Owner o;

	LinkInfo linfo;

	auto it1 = linksPlayerOne.find(name);
	auto it2 = linksPlayerTwo.find(name);
	if (it1 != linksPlayerOne.end()) {
		o = Owner::playerOne;
		linfo = it1->second;
	} else {
		o = Owner::playerTwo;
		linfo = it2->second;
	}

	int size = firewallpair.size();
	int i = 0;
	Pos p = {pos_x, pos_y};

	for(; i < size; i++) {
		if (firewallpair[i].first == p) {
			if (firewallpair[i].second == 'm') {
				if (o == Owner::playerTwo) { 
					linfo.hidden = false;
                                        cout << "Info for Link " << name << ':' << endl;
		                        cout << linfo.type << linfo.strength << endl;
                                }

			} else {
				if (o == Owner::playerOne) {
					linfo.hidden = false;
                                        cout << "Info for Link " << name << ':' << endl;
		                        cout << linfo.type << linfo.strength << endl;
                                }
			}
 		}
		break;
	}

}


TextDisplay::~TextDisplay(){

}


ostream &operator<<(ostream &out, TextDisplay& td){

	cout << "Player 1: " << endl;

	cout << "Downloaded: " << td.dDownloadedP1 << "D, " << td.vDownloadedP1 << "V" << endl; 
	cout << "Abilities: " << td.numAbilitiesRemP1  << endl;

	td.printPiecesPlayerOne(); 


	for (int i = 0; i < 8; i ++){
		cout << "="; 
	}

	cout << endl; 

	for (int i = 0; i < 8; i ++){
		for (int j = 0; j < 8; j ++){
			cout << td.theBoard[i][j]; 
		}
		cout << endl; 
	} 

	for (int i = 0; i < 8; i ++){
		cout << "="; 
	}
	cout << endl; 

	cout << "Player 2: " << endl; 

	cout << "Downloaded: " << td.dDownloadedP2 << "D, " << td.vDownloadedP2 << "V" << endl; 
	cout << "Abilities: " << td.numAbilitiesRemP2 << endl; 

	td.printPiecesPlayerTwo(); 
}


