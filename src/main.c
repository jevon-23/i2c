#include "../include/client.h"
#include "../include/clock.h"
#include "../include/components.h"
#include "../include/host.h"
#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int processCLI(int argc, char **argv, char **filename) {
  if (argc != 2) {
    printf("usage:");
    exit(-1);
  }
  return 0;
}

int main(int arg, char *argv[]) {

  bool **busses = (bool **)malloc(sizeof(bool) * 2);
  *busses = (bool *)malloc(sizeof(bool));
  *(busses + 1) = (bool *)malloc(sizeof(bool));

  bool *sda = *busses;
  bool *scl = *busses + 1;

  *sda = true;
  *scl = true;

  enum rw *rw_bus = (enum rw *)malloc(sizeof(enum rw));
  *rw_bus = READ;

  // clock *clock = new_clock(scl);
  message *msg = new_message();

  component *ch = new_component(0x0, sda, scl, msg, rw_bus);
  component *cs1 = new_component(0xff, sda, scl, msg, rw_bus);
  component *cs2 = new_component(0x24, sda, scl, msg, rw_bus);

  client *c1 = new_client(cs1);
  client *c2 = new_client(cs2);

  client **cc = (client **)malloc(sizeof(client *) * 2);
  *cc = c1;
  *(cc + 1) = c2;

  host *h = new_host(ch, cc, 2);

  h->comp->w_buffer[0] = 0xbe;
  h->comp->w_buff_len++;
  h->comp->w_buffer[1] = 0x7f;
  h->comp->w_buff_len++;

  for (int i = 0; i < h->num_clients; i++) {
    printf("address: %d\n", (*(h->clients + i))->comp->address);
  }

  send_message(h);

  printf("send message finished\n");
  printf("Successful build!\n");
  return 0;
}
