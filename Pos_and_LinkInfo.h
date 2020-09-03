#ifndef PL_H
#define PL_H

using namespace std;

struct Pos{
	int x; 
	int y;
	bool operator==(Pos p) {
		return (this->x == p.x && this->y == p.y);
	}
};

struct LinkInfo{
	char type;
	int strength;
	bool hidden;
};

#endif
