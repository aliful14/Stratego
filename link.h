#ifndef LINK_HEADER
#define LINK_HEADER
#include <iostream>
#include "subject.h"
#include "observer.h"
#include "link.h"
#include "info.h"
#include "state.h"

class Link: public Subject<Info, State>, public Observer <Info, State> {

	Owner ownedBy; 
	LinkType type;  

	public: 
	
	int row; 
	int col; 

	int strength; 
	const char name;
        bool isBoost = false;
        
        bool isFrozen = false;  
	int frozenClock = 2; 	

	Info lastOpponent;  

	Link(Owner ownedBy, LinkType type, int r, int c, int strength, char name); 
	void notify(Subject<Info, State> &whoFrom) override; 
	Info getInfo() const override;
	Info getOpponentInfo() const override; 
	void move(Direction d, bool battleInitiation=false, bool downloadByFW=false);
	void boost_move(Direction d, bool battleInitiation=false, bool downloadByFW=false);

	friend std::ostream &operator<<(std::istream &out, Link l); 	

}; 

#endif 
