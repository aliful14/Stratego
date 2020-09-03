#include "graphicDisplay.h"
#include "window.h"
#include "link.h"

using namespace std;

GraphicDisplay::GraphicDisplay(): xw{new Xwindow} {
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

void GraphicDisplay::printPiecesPlayerOne() {
	int x = 10;
	int y = 35;
	int w = 20;
	int h = 20;

	for (const auto &myPair: linksPlayerOne) {
		string s(1, myPair.first);
		string strength = to_string(myPair.second.strength);
		s += ':';
		xw->drawString(x, y, s);
		if ((myPair.second.hidden) && (currentPlayer == Owner::playerTwo)) {
			xw->drawString(x+20, y, "?");
		} else {
			if (myPair.second.type == 'V')
				xw->fillRectangle(x+20, y-10, w, h, Xwindow::Red);
			else 
				xw->fillRectangle(x+20, y-10, w, h, Xwindow::Green);

			xw->drawString(x+27, y+3, strength);
		}

		x += 60;
	}
}

void GraphicDisplay::printPiecesPlayerTwo() {
	int x = 10;
	int y = 490;
	int w = 20;
	int h = 20;

	for (const auto &myPair: linksPlayerTwo) {
		string s(1, myPair.first);
		string strength = to_string(myPair.second.strength);
		s += ':';
		xw->drawString(x, y, s);
		if ((myPair.second.hidden) && (currentPlayer == Owner::playerOne)) {
			xw->drawString(x+20, y, "?");
		} else {
			if (myPair.second.type == 'V')
				xw->fillRectangle(x+20, y-10, w, h, Xwindow::Red);
			else 
				xw->fillRectangle(x+20, y-10, w, h, Xwindow::Green);

			xw->drawString(x+27, y+3, strength);
		}

		x += 60;
	}
}






void GraphicDisplay::notify(Subject <Info, State> &whoFrom){ 
          
	State linkState = whoFrom.getState(); 
	Info linkInfo = whoFrom.getInfo(); 

	if (linkState.state == StateType::NewPiece){
                currentPlayer == Owner::playerOne;

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

	updateGraphic();
}

void GraphicDisplay::updateBoard(char name) { 
	int pos_x = positionMapping[name].x; 
	int pos_y = positionMapping[name].y; 
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

void GraphicDisplay::updateGraphic() {
	int size_sq = 50;
	int size_l = 30;
	int l_colr = Xwindow::White;

	for (int i = 0; i < 10; i++) xw->fillRectangle(i*50, 0, size_sq, size_sq, Xwindow::White);

	string DP1 = "Downloaded: "  + std::to_string(dDownloadedP1) + "D, " 
		+ std::to_string(vDownloadedP1) + "V";

	string AB1 = "Abilities: " + std::to_string(numAbilitiesRemP1);


	xw->drawString(10, 15, "Player 1:");
	xw->drawString(100, 15, DP1);
	xw->drawString(300, 15, AB1);

        printPiecesPlayerOne();

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; j++) {
			int cor_x = 50 * (j + 1);
			int cor_y = 50 * (i + 1);
			int l_x = cor_x + 10;
			int l_y = cor_y + 10;

			int sq_colr;
			if ((i == 0 && (j == 3 || j == 4)) ||
					(i == 7 && (j == 3 || j == 4)))
				sq_colr = Xwindow::Black;
			else if ((i % 2 == 0 && j % 2 == 0) || (i%2 == 1 && j%2 == 1))
				sq_colr = Xwindow::Blue;
			else sq_colr = Xwindow::Green;

			xw->fillRectangle(cor_x, cor_y, size_sq, size_sq, sq_colr);

			char c = theBoard[i][j];
			string s(1, c);


			int size = firewallpair.size();
			int n = 0;
			Pos p = {i, j};
			for(; n < size; n++) {
				if (firewallpair[n].first == p) {
					xw->fillRectangle(cor_x, cor_y, size_sq, size_sq, Xwindow::Red);
					string s(1, firewallpair[n].second);
					xw->drawString(cor_x+7, cor_y+7, s);
					break;
				}
			}
			if (c != '.' && c != 'S' && c != 'm' && c != 'w') {
				xw->fillRectangle(l_x, l_y, size_l, size_l, l_colr);
				xw->drawString(l_x + 15, l_y + 15, s);
			}
		}
	}

        for (int i = 0; i < 10; i++) xw->fillRectangle(i*50, 450, size_sq, size_sq, Xwindow::White);

	string DP2 = "Downloaded: "  + std::to_string(dDownloadedP2) + "D, " 
		+ std::to_string(vDownloadedP2) + "V";

	string AB2 = "Abilities: " + std::to_string(numAbilitiesRemP2);


	xw->drawString(10, 470, "Player 2:");
	xw->drawString(100, 470, DP2);
	xw->drawString(300, 470, AB2);


	printPiecesPlayerTwo();

}	


GraphicDisplay::~GraphicDisplay(){
	delete xw;
}


