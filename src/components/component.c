#include "../../include/components.h"
#include <stdio.h>
#include <stdlib.h>

/* Message functionality */
message *new_message() {
  message *msg = (message *)malloc(sizeof(message));
  msg->state = (enum msg_state *)malloc(sizeof(enum msg_state));
  msg->msg = (uint32_t *)malloc(sizeof(uint32_t));
  msg->addr = 0;
  reset_message_bus(msg);
  return msg;
}

void reset_message_bus(message *msg) {
  *(msg->state) = START;
  *msg->msg = 0x0;
  msg->addr = 0x0;
}

enum rw *new_rw() {
  enum rw *r_w = (enum rw *)malloc(sizeof(enum rw));
  *r_w = READ;
  return r_w;
}

/* Componoent functionality */
component *new_component(uint8_t address, bool *sda, bool *scl,
                         message *msg_bus, enum rw *rw_bus) {
  component *c = (component *)malloc(sizeof(component));

  if (address > 0x7f) {
    printf("reset address to something below 0x7f, invalid address: %x\n",
           address);
    exit(-1);
  }
  c->address = address;
  c->sda = sda;
  c->scl = scl;
  c->msg = msg_bus;
  c->r_w = rw_bus;
  return c;
}

void comp_toggle_scl(component *comp) { *comp->scl = !(*comp->scl); }

void comp_set_sda(component *comp, bool bit) { *comp->sda = bit; }

void comp_update_message(component *comp, bool bit) {
  *comp->msg->msg = (*(comp->msg->msg) << 1) | bit;
}

void comp_update_message_state(component *comp, enum msg_state state) {
  *comp->msg->state = state;
}

void comp_set_rw_state(component *comp, enum rw r_w) { *comp->r_w = r_w; }
