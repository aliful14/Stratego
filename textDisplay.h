#ifndef TEXT_DISPLAY_GUARD
#define TEXT_DISPLAY_GUARD
#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include "state.h"
#include "info.h"
#include "observer.h"
#include "Pos_and_LinkInfo.h"


using namespace std; 


class TextDisplay: public Observer <Info, State>{
	
        std::vector<std::vector<char>> theBoard; 
	std::map<char, Pos> positionMapping;
	std::vector< pair <Pos, char> > firewallpair;

	std::map<char, LinkInfo> linksPlayerOne; 
	std::map<char, LinkInfo> linksPlayerTwo; 

	int dDownloadedP1; 
	int dDownloadedP2; 
	int vDownloadedP1; 
	int vDownloadedP2; 

	int numAbilitiesRemP1 = 5; 
	int numAbilitiesRemP2 = 5; 

	public:

	Owner currentPlayer; 

	TextDisplay(); 
	~TextDisplay(); 
	void notify(Subject<Info, State> &whoFrom) override; 
	void updateBoard(char linkName); 
	void printPiecesPlayerOne(); 
	void printPiecesPlayerTwo(); 
	friend std::ostream&operator<<(std::ostream &out, TextDisplay &td); 
}; 



#endif 
