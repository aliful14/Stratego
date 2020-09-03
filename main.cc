#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "game.h"
using namespace std;

void main2(Game &newGame, string g); 

int main(int argc, char *argv[]) {
   
  cout << "Number of optional arguments" << argc - 1 << endl;    
 
  string player1Sequence = "V1V2V3V4D1D2D3D4"; 
  string player2Sequence = player1Sequence; 

  string player1Abilities = "LFDSP"; 
  string player2Abilities = player1Abilities; 

  bool graphicalOption = false; 

  for (int i = 1; i < argc; i ++){
	  cout << "Current argument: " << argv[i] << endl;  

	  string link1 = "-link1"; 
	  string link2 = "-link2"; 
	  string ability1 = "-ability1"; 
	  string ability2 = "-ability2"; 
	  string graphicalInterface = "-graphics"; 

	  if (graphicalInterface.compare(argv[i]) == 0){
		  cout << "Graphic Interface command received" << endl; 
		  graphicalOption = true; 
		  continue; 
	  }

	  if (link1.compare(argv[i]) == 0){
		  if (i < argc - 1){ 
		  	player1Sequence = argv[i + 1]; 
		 } 
	  } else if (link2.compare(argv[i]) == 0){
		  if (i < argc - 1){
		  	player2Sequence = argv[i + 1]; 
		  }
	  } else if (ability1.compare(argv[i]) == 0){
		  if (i < argc - 1){
		  	player1Abilities = argv[i + 1]; 
		 }
	  } else if (ability2.compare(argv[i]) == 0){
		  if (i < argc - 1){
		  	player2Abilities = argv[i + 1]; 
		  } 
	  }

	  i++; 
  }

  //cout << "player1Sequence: " << player1Sequence << endl; 
  //cout << "player2Sequence: " << player2Sequence << endl; 
  //cout << "player1Abilities: " << player1Abilities << endl; 
  //cout << "player2Abilities: " << player2Abilities << endl; 

  Game newGame{player1Sequence, player1Abilities, player2Sequence, player2Abilities, graphicalOption}; 
  newGame.currentPlayer = Owner::playerOne; 

  string cmd; 

  bool playerOneTurn = true; 

  while (!newGame.isGameOver() && (cin >> cmd)) { 

	if (cmd == "quit") {
		break; 
	} else if (cmd == "sequence") {

		string fileName; 
		cin >> fileName; 

		ifstream myfile (fileName); 

		if (myfile.is_open()){
			string line; 
			while (getline(myfile, line)){
				main2(newGame, line); 
			}
		} else {
			cout << "Unable to open the god-damn file." << endl; 
		
		}

		myfile.close(); 
	}
	else {
		main2(newGame, cmd); 
	}
  }

  if (newGame.isGameOver()){
  	cout << "**********************" << endl; 
  	if (newGame.whoWon() == Owner::playerOne){
		cout << "Player One won!!" << endl; 
	} else {
		cout << "Player Two won!!" << endl; 
	}
	cout << "**********************" << endl; 
  }
}

   void main2(Game &newGame, string cmd){ 

    if (cmd.substr(0, 4) == "move") {

	 char linkName; 
	 string direction; 
	 
	 if (cmd == "move") {

	 	cin >> linkName; 
	 	cin >> direction; 

	 } else {
		
		istringstream line(cmd);
		vector<string> inputs; 
		string input; 
		while (getline(line, input, ' ')){
			//cout << "input read in: " << input << endl; 
		        inputs.push_back(input);  
		}
		
		linkName = inputs[1][0]; 
		direction = inputs[2]; 
	 }
	
	 char linkNameUpper = toupper(linkName); 

	 if ((linkNameUpper >= 65) && (linkNameUpper <= 72)){
		 //cout << "Valid link" << endl; 
	 } else {
		 //cout << "Not a valid link" << endl;
		 //cout << "Link: " << linkName << endl;  
		 return; 
	 }

	 if(newGame.isInactivePlayer(linkName)){
		 cout << "Player is inactive at the moment" << endl; 
		 return; 
	 };  
	
	 if ((direction == "up") || (direction == "down" || (direction == "left") || (direction == "right"))){
		 //cout << "Valid Move" << endl; 

		 Owner currentPlayer = Owner::playerTwo; 

		 if (direction == "down"){ 
			 newGame.move(linkName, Direction::Down); 
		 } else if (direction == "up") {
			 newGame.move(linkName, Direction::Up); 
		 } else if (direction == "left"){
			 newGame.move(linkName, Direction::Left); 
		 } else {
			 newGame.move(linkName, Direction::Right); 
		 }
		
	 } 	
		 
    }

    else if (cmd == "abilities"){

	   newGame.printAbilities(); 

    } 

    // ****************** Modified Code Begins ********************
 
    else if (cmd.substr(0, 7) == "ability"){

	    int num; 
	    string abilityArgs; 

    	if (cmd == "ability") {

	    	cin >> num; 
	    	cin >> std::ws;
	    	getline(cin, abilityArgs); 

    	} else {
		
		istringstream line(cmd); 
		vector<string> inputs; 
		string input; 
		while (getline(line, input, ' ')){
			inputs.push_back(input); 
			//cout << "Read in: " << input << endl; 
		}

		istringstream line2(inputs[1]); 
		line2 >> num; 

		if (inputs.size() > 2){
			abilityArgs = inputs[2]; 
		}

		if (inputs.size() > 3){
			//cout << "4 inputs passed in" << endl; 
			abilityArgs = abilityArgs + " " + inputs[3]; 
		}

	}
	
	cout << "Ability Args: " << abilityArgs << endl; 
	newGame.useAbility(num, abilityArgs); 
    }

    // ********************* Modified Code ends **********************

    else if (cmd == "board"){
	    cout << newGame; 
    } 
    
  }
