#include "../../include/clock.h"
#include <stdlib.h>

clock *new_clock(bool *scl) {
  clock *c = (clock *)malloc(sizeof(clock));
  c->timer = 0;
  return c;
}
