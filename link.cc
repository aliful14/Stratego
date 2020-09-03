#include "link.h"
#include <iostream>
using namespace std; 

void Link::notify(Subject<Info, State> &whoFrom){	
	
	State linkState = whoFrom.getState(); 
	Info linkInfo = whoFrom.getInfo(); 

        // ***************************** Modified Code Begins Here *******************//
	if (linkState.state == StateType::JustMoved){
		if (isFrozen){
			frozenClock -= 1; 
			if (frozenClock == 0){
				isFrozen = false; 
				//cout << "Unfreezing myself" << endl; 
			} 
		}
	}

	if ((linkState.state == StateType::DownloadedByPlayerOne) || (linkState.state == StateType::DownloadedByPlayerTwo)){
		if (isFrozen){
			frozenClock -= 1; 
			if (frozenClock == 0){
				isFrozen = false; 
			}
		}
	} 

	if (linkState.state == StateType::InitiateBattle){
		
		if (isFrozen){
			frozenClock -= 1;
			if (frozenClock == 0){
				//cout << "Unfreezing myself" << endl; 
				isFrozen = false; 
			}
		} 


		// ************************** Modified Code Ends Here **************************

		//cout << "I am link named:" << name << "with pos - x: " << row << " and pos - y: " << col << endl; 

		if ((row == linkInfo.row) && (col == linkInfo.col)){
			
			lastOpponent = linkInfo; 
			
			//cout << "Battle cry heard" << endl; 

			if (strength > linkInfo.strength){ 
				//cout << "Battle Won: " << endl; 
			        setState({StateType::WonBattle, Direction::Up}); 	
			} else {
				//cout << "Battle Lost" << endl; 
				setState({StateType::LostBattle, Direction::Up}); 
			}

			notifyObservers(); 

		}

	}

        if (linkState.state == StateType:: Polarize) {
		if(linkInfo.name == name) {
			if(type == LinkType::data){
				type =LinkType::virus;

			} else {
				type = LinkType::data;
			}
		}

	}

        // ************** Modified Code Begins Here **************************
	 
	if (linkState.state == StateType::QuickFreeze){
		if (linkInfo.name == name){
			//cout << "Freezing myself" << endl; 
			isFrozen = true; 
			frozenClock = 2; 
		}
	}

	// ********************* Modified Ends Here **************************


        if (linkState.state == StateType:: Download) {
		//cout << "ENTERED DOWNLOAD BLOCK" << endl;
		if(linkInfo.ownedBy == Owner::playerOne) {
			//cout << "ENTERED P1 block" << endl;
			///cout << "Char =" << linkInfo.name << endl;
			//cout << "link type =" << linkInfo.type << endl;
			if(linkInfo.name == name) {
				setState({StateType::DownloadedByPlayerOne});
				///cout <<"Setting State from P1 block" << endl;
				notifyObservers();
				///cout << "Called NotifyObservers()" << endl;
			}
		} 
		else if(linkInfo.ownedBy == Owner::playerTwo) {
			//cout << "Entered P2 block" << endl;
			///cout << "Char =" << linkInfo.name << endl;
			if(linkInfo.name == name) {
				setState({StateType::DownloadedByPlayerTwo});
				///cout << "Setting State from P2 block" << endl;
				notifyObservers();
				///cout << "Called NotifyObservers()" << endl;
				}
		}
		//cout << "calling notifyObservers()" << endl;
		//notifyObservers();
	}

	if (linkState.state == StateType:: Intensify) {
		if(linkInfo.name == name) {
			if ((type == LinkType::data) || (type == LinkType::virus)){
				strength += 1;

			} 
		}

	}

	if(linkState.state == StateType:: Antidote){
		if(linkInfo.ownedBy == Owner::playerOne){
				setState({StateType::AntidoteP1});
				notifyObservers();
			
		}

		 if (linkInfo.ownedBy == Owner::playerTwo) {
			setState({StateType::AntidoteP2});
			notifyObservers();
		 }
		
	}








/*
	if(linkState.state == StateType::Intensify){
		if(linkInfo.ownedBy == Owner::playerOne){
			if(linkInfo.name == name){
				setState({StateType::IP1});
				notifyObservers();
			}
		}
		if(linkInfo.ownedBy == Owner::playerTwo){
			if(linkInfo.name == name){
				setState({StateType::IP2});
				notifyObservers();
			}
		}
	}
	*/
}
Link::Link(Owner ownedBy, LinkType type, int r, int c, int strength, char name): ownedBy {ownedBy}, type {type}, row {r}, col {c}, strength {strength}, name {name} {

	setState({StateType::NewPiece, Direction::Up}); 

};

void Link::move(Direction d, bool battleInitiation, bool downloadByFW){
	
	StateType t = StateType::JustMoved;

        if (downloadByFW) {
		cout << "Link " << name << " is downloaded by Firewall." << endl;
		//couf << "The strength of Link " << name << " is " << strength << endl;
		if (ownedBy == Owner::playerOne)
			t = StateType::DownloadedByPlayerOne;
		else t = StateType::DownloadedByPlayerTwo;
                setState({t, d});
                notifyObservers();
                return;
	}
 
	
	if (ownedBy == Owner::playerOne){
		if (d == Direction::Down){

			if (row == 7){
				t = StateType::DownloadedByPlayerOne; 
			} else {
				row ++; 
			}

		} else if (d == Direction::Up){		
			row --; 
		} else if (d == Direction::Left){
			col --; 
		} else {
			col ++; 
		}

		if ((row == 7) && ((col == 3 || col == 4))){
			t = StateType::DownloadedByPlayerTwo; 
		}

	}  else if (ownedBy == Owner::playerTwo){

		if (d == Direction::Up){

			if (row == 0){
				t = StateType::DownloadedByPlayerTwo; 
			        //cout << "Downlaoded by player two" << endl; 	
			} else {
				row --; 
			}

		} else if (d == Direction::Down){
			row ++; 
		} else if (d == Direction::Left){
			col --; 
		} else {
			col ++; 
		}

		if ((row == 0) && (col == 3 || col == 4)){
			t = StateType::DownloadedByPlayerOne; 
		}
	}

	if (battleInitiation){
		//cout << "Battle Initiated" << endl; 
		t = StateType::InitiateBattle;
	}
	
	setState({t, d}); 
	notifyObservers(); 
}

void Link::boost_move(Direction d, bool battleInitiation, bool downloadByFW) {
	StateType t = StateType::JustMoved;
        
        if (downloadByFW) {
		cout << "Link " << name << " is downloaded by Firewall." << endl;
		//couf << "The strength of Link " << name << " is " << strength << endl;
		if (ownedBy == Owner::playerOne)
			t = StateType::DownloadedByPlayerOne;
		else t = StateType::DownloadedByPlayerTwo;
                setState({t, d});
                notifyObservers();
                return;
	}


	if (ownedBy == Owner::playerOne) {
		if (d == Direction::Down) {
			if (row == 6 || row == 7) {
				t = StateType::DownloadedByPlayerOne;
			} else {
				row += 2;
			}
		} else if (d == Direction::Up) {
			row -= 2;
		} else if (d == Direction::Left) {
			col -= 2;
		} else {
			col += 2;
		}

		if ((row == 7) && (col == 3 || col == 4)) {
			t = StateType::DownloadedByPlayerTwo;
		}
	} else if (ownedBy == Owner::playerTwo) {
		if (d == Direction::Up) {
			if (row == 1 || row == 0) {
				t = StateType::DownloadedByPlayerTwo;
			} else {
				row -= 2;
			}
		} else if (d == Direction::Down) {
			row += 2;
		} else if (d == Direction::Left) {
			col -= 2;
		} else {
			col += 2;
		}

		if ((row == 0) && (col == 3 || col == 4))
			t = StateType::DownloadedByPlayerOne;
	}

	if (battleInitiation) {
		t = StateType::InitiateBattle;
	}

	setState({t, d});
	notifyObservers();
}


Info Link::getInfo() const {
	return {ownedBy, type, row, col, strength, name, isBoost}; 
}

Info Link::getOpponentInfo() const {
	return lastOpponent; 
}
