#ifndef CLOCK_H
#define CLOCK_H

#include <inttypes.h>
#include <stdbool.h>

typedef struct clock {
    uint64_t timer;
    bool *scl;
}clock;

clock *new_clock(bool *scl);

#endif
