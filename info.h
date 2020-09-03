#ifndef __INFO_H__
#define __INFO_H__
#include <cstddef>
#include "state.h"

struct Info {
  Owner ownedBy; 
  LinkType type; 
  int row, col, strength; 
  char name; 
  bool boosted;
};

#endif
