#ifndef HOST_H
#define HOST_H
#include "components.h"
#include "client.h"

/* A host can drive sda high and low */
typedef struct host {
  component *comp;
  client **clients;
  int num_clients;
} host;


host *new_host(component *comp, client **clients, int num_clients);

void send_message(host *h);
void update(host *h);
#endif
