#include "player.h" 
#include "link.h"
#include <sstream>

using namespace std; 


void Player::useAbility(int num, string arguments){

	cout << "I am using ability with title: " << abilitySequence[num-1] << endl; 

        
	char ab = abilitySequence[num-1];

	if (numAbilitiesRemaining == 0) return;

	if (abilitiesUsed[num-1]) {
		cout << "ability already used" << endl;
                return;
	} else {
		abilitiesUsed[num-1] = true;
		numAbilitiesRemaining--;
	}

        if (ab == 'L') {
		char lname = arguments[0];

		for (int i = 0; i < 8; i++) {
			if (lname == theLinks[i]->name) {
				theLinks[i]->isBoost = true;
				break;
			}
	
		}
		setState({StateType::Boost});

	} else if (ab == 'F') {
                istringstream iss(arguments);
		int r, c;
		iss >> r >> c;
		setState({StateType::Firewall, Direction::Up, {r, c}});

	} else if (ab == 'D') {

		char lname = arguments[0]; 
		setState({StateType::Download}); 
		set_lastChar(lname); 

	} else if (ab == 'S') {
		cout << "debug :" << ab  << endl;
		char lname = arguments[0];
		setState({StateType::Scan});
		set_lastChar(lname);

	} else if (ab == 'P') {
		char lname = arguments[0];
                      cout << "This is a character being polarized : " << lname << endl;
		      setState({StateType::Polarize});
                      set_lastChar(lname);
		

	} else if (ab == 'Q'){
		char lname = arguments[0]; 
		//cout << "This is the character being frozen : " << lname << endl; 
		setState({StateType::QuickFreeze}); 
		set_lastChar(lname); 

	} else if (ab == 'I') {
		char lname = arguments[0];
		setState({StateType::Intensify});
		set_lastChar(lname);
	} else if (ab == 'A') {
		char lname = arguments[0];
		setState({StateType::Antidote});
		set_lastChar(lname);
	}

	notifyObservers();

}

void Player::printAbilities() const {

	for (int i = 0; i < 5; i ++){
		//cout << "Ability:  " << abilitySequence[i] << " is ";
		cout << "Ability" << " " << (i+1) << " " << ":" << " " << abilitySequence[i] << " is "; 

		if (abilitiesUsed[i]){
			cout << "used up." << endl; 
		} else {
			cout << "not used up." << endl; 
		}
	}
	
}

Posi Player::getLinkPos(char name){
	
	int index; 

	for (int i = 0; i < theLinks.size(); i ++){
		if (theLinks[i]->name == name){
			index = i; 
			break; 
		}
	}

	return {theLinks[index]->row, theLinks[index]->col}; 

}

bool Player::is_boost(char name) {

	int index;

	for (int i = 0; i < theLinks.size(); ++i)
		if (theLinks[i]->name == name) {
			index = i;
			break;
		}
	return theLinks[index]->isBoost;
}


void Player::notify(Subject<Info, State> &whoFrom){
	State s = whoFrom.getState(); 
	Info info = whoFrom.getInfo(); 

	if (s.state == StateType::DownloadedByPlayerOne){

		if (playerType == Owner::playerOne){
		
			if (info.type == LinkType::virus){
				virusDownloaded++; 
			} else {
				dataDownloaded++; 
			}
		}
	} 


	if (s.state == StateType::DownloadedByPlayerTwo){
					
		if (playerType == Owner::playerTwo){
			
			//cout << "Increment Operation" << endl;

			if (info.type == LinkType::virus){
				virusDownloaded++; 
			} else {
				dataDownloaded++; 
			}

		}
	}

	if (s.state == StateType::WonBattle){ 
		if (info.ownedBy == playerType){
			if (whoFrom.getOpponentInfo().type == LinkType::virus){

				//cout << "Virus downlaoded" << endl; 

				virusDownloaded++; 
			} else {

				//cout << "Data downloaded" << endl; 
				dataDownloaded++; 
			}
		}
	}

	if (s.state == StateType::LostBattle){ 
		if (whoFrom.getOpponentInfo().ownedBy == playerType){
			//cout << "Player called" << endl; 
			if (info.type == LinkType::virus){
				//cout << "Virus downloaded" << endl; 
				virusDownloaded++; 
			} else {
				//cout << "Data downloaded" << endl; 
				dataDownloaded++; 
			}
		}
	}

	if (s.state == StateType::AntidoteP1) {
		if (playerType == Owner:: playerOne) {
			virusDownloaded--;
		}
	}

	if (s.state == StateType::AntidoteP2) {
		if (playerType == Owner:: playerTwo) {
			virusDownloaded--;
		}
	}

}

Player::~Player(){
	for (int i = 0; i < theLinks.size(); i ++){
		delete theLinks[i]; 
	}
}
Player::Player(Owner pType, string sequenceLinks, string sequenceAbilities): playerType {pType}, abilitySequence {sequenceAbilities} {
	
	numAbilitiesRemaining = 5; 
	abilityUsedThisTurn = false; 

	for (int i = 0; i < 5; i ++){
		abilitiesUsed[i] = false; 
	}

	for (int i = 0; i < sequenceLinks.length(); i ++){
			
		int r, c; 
		int asciiCode; 

		c = i / 2; 

		if (pType == Owner::playerOne){
			r = 0; 
			asciiCode = 97;  
		} else {
			r = 7;
		        asciiCode = 65; 	
		}

		if ((i == 6) || (i == 8)){	
			if (pType == Owner::playerOne){
				 r += 1;  
			} else {
				r -= 1; 
			}
		}

		LinkType l; 

		if ((sequenceLinks[i] == 'V') || (sequenceLinks[i] == 'v')){
			l = LinkType::virus; 
		} else {
			l = LinkType::data; 
		}
		
		theLinks.push_back(new Link{pType, l, r, c, sequenceLinks[i+1] - 48, asciiCode + c});
		i ++; 
	}
};

void Player::move(char name, Direction d, bool battleInitiation, bool downloadByFW){
        bool boost = is_boost(name);

	abilityUsedThisTurn = false; 	

	for (int i = 0; i < 8; i ++){
		if (theLinks[i]->name == name){
			if (boost) theLinks[i]->boost_move(d, battleInitiation, downloadByFW);
			else theLinks[i]->move(d, battleInitiation, downloadByFW);
		        break; 	
		}
	}
}

void Player::set_lastChar(char c) {
	lastChar = c;
}

Info Player::getOpponentInfo() const {
	return lastOpponent;
}

Info Player::getInfo() const {
	LinkType t = LinkType::data;
	int r = 1;
	int col = 1;
	int strength = 1;
	Owner O = playerType;
	return {O, t,r,col,strength,lastChar};
}

