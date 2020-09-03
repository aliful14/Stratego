#ifndef PLAYER_GUARD
#define PLAYER_GUARD
#include "state.h"
#include "info.h"
#include <vector>
#include <string>
#include "observer.h"
#include "posi.h"
#include "subject.h"

class Link; 
class Player: public Observer<Info, State>, public Subject<Info, State>{

	Owner playerType; 
		
	public:
	
	std::string abilitySequence;

	// Modified Code Begins // 	
	int dataDownloaded = 0; 
	int virusDownloaded = 0;
        bool abilityUsedThisTurn; 	
	// Modified Code Ends // 

	int numAbilitiesRemaining;
	char lastChar;
	Info lastOpponent;

	std::vector <Link*> theLinks; 
	bool abilitiesUsed[5]; 

	Player(Owner oType, std::string seqLinks, std::string seqAbilities);  
	~Player(); 
	void notify(Subject <Info, State> &whoFrom) override;
	Info getInfo() const override;
	Info getOpponentInfo() const override;
	void move(char a, Direction d, bool battleInitiation=false, bool downloadByFW=false); 
	void useAbility(int num, std::string arguments); 
	void printAbilities() const;
	void set_lastChar(char c);

	Posi getLinkPos(char);
       	bool is_boost(char);
	 
}; 

#endif
