#ifndef STATE_H
#define STATE_H

enum class StateType {NewPiece, JustMoved, InitiateBattle, WonBattle, LostBattle, DownloadedByPlayerOne, DownloadedByPlayerTwo, Boost, Firewall, Scan, NoState, Polarize, Download, QuickFreeze, 
Intensify,Antidote, AntidoteP1, AntidoteP2};
enum class Direction {Up, Down, Left, Right};
enum class Owner {playerOne, playerTwo}; 
enum class LinkType {virus, data}; 


struct fPos {
	int x;
	int y;
};

struct State {
  StateType state;  
  Direction direction;
  fPos FWpos;
};

#endif
