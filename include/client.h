#ifndef CLIENT_H
#define CLIENT_H
#include "components.h"

/* A client cannot drive sda high, can only pull it low 
 *  Pull down on 0 when writing
 */
typedef struct client {
    component *comp;
    bool chosen;
} client;

client *new_client(component *comp);
void update_client(client *c);
void print_client(client *c);
#endif

