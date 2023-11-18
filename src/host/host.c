#include "../../include/host.h"
#include <stdio.h>
#include <stdlib.h>

/* A host can drive sda high and low
 *  for reads, leave sda high and take the pull downs on every interval
 *  as a 0
 *  otherwise 1
 *
 */
host *new_host(component *comp, client **clients, int num_clients) {
  host *h = (host *)malloc(sizeof(host));
  h->comp = comp;
  h->clients = clients;
  h->num_clients = num_clients;
  return h;
}

void print_state(host *h) {
  printf("sda: %d\n", *h->comp->sda);
  printf("scl: %d\n", *h->comp->scl);
  printf("msg: 0x%08x\n", *h->comp->msg->msg);
  printf("msg_state: %x\n", *h->comp->msg->state);
  printf("msg_addr: %x\n", h->comp->msg->addr);
  printf("rw: %d\n", *h->comp->r_w);

  printf("printing client's states:\n");
  for (int i = 0; i < h->num_clients; i++) {
    print_client(*(h->clients + i));
  }
}

void update_host_clients(host *h) {
  for (int i = 0; i < h->num_clients; i++) {
    update_client(*(h->clients + i));
  }
}

void update(host *h) {
  switch (*(h->comp->msg->state)) {
  case START:
    if (!*(h->comp->sda) && *h->comp->scl)
      /* if sda is low before scl is low, beginning of start message */
      goto update_state;
    if (!*(h->comp->sda) && !*h->comp->scl) {
      comp_update_message_state(h->comp, EXP_ADDR);
      goto update_state;
    }
    break;
  case EXP_ADDR:
    h->comp->msg->addr = (h->comp->msg->addr << 1) | *h->comp->sda;
    break;
  case RW:
    /* Clock should be low when entering */
    comp_toggle_scl(h->comp);
    update_host_clients(h);
    comp_update_message_state(h->comp, ACK_NAK1);
    comp_toggle_scl(h->comp);

    break;
  case ACK_NAK1:
    /* SDA gets pulled down by client as an ack */
    if (*h->comp->sda) {
      printf("did not find a client\n");
      exit(-1);
    }
    comp_update_message_state(h->comp, DATA1);
    break;

  case ACK_NACK2:
    /* SDA gets pulled down by client as an ack */
    if (*h->comp->sda) {
      printf("Data did not reach client, print address\n");
      exit(-1);
    }
    comp_update_message_state(h->comp, DATA2);
    printf("acknowledged data 1, updated 4 data 2\n");
    break;
  case ACK_NACK3:
    /* SDA gets pulled down by client as an ack */
    if (*h->comp->sda) {
      printf("Data did not reach client, print address\n");
      exit(-1);
    }
    comp_update_message_state(h->comp, STOP);
    printf("acknowledged data 2, updated 4 stop\n");
    break;

  case STOP:
    if (*h->comp->scl && !*h->comp->sda) {
      break;
    }
    if (*h->comp->scl && *h->comp->sda) {
      printf(
          "\n\nFinished message, printing final state b4 restarting bus\n\n");
      print_state(h);
      reset_message_bus(h->comp->msg);
      printf("message bus reset\n");
      print_state(h);

      break;
    }
  case DATA1:
  case DATA2:
    break;
  default:
    printf("invalid state\n");
    exit(-1);
  }
  // update_host_clients(h);
update_state:
  /* Update the message */
  comp_update_message(h->comp, *h->comp->sda);
}

void send_data(host *h, uint8_t data, int len);

void start_message(host *h) {
  /* The host sends the start of the message to all clients by turning sda
   * from high to low, before switching scl from high to low */
  comp_set_sda(h->comp, false);
  update(h);
  comp_toggle_scl(h->comp);
  update(h);
}

void end_message(host *h) {
  /* The host sends the start of the message to all clients by turning sda
   * from high to low, before switching scl from high to low */
  comp_toggle_scl(h->comp);
  update(h);
  comp_set_sda(h->comp, true);
  update(h);
}

void send_address(host *h, uint8_t address) {
  /* Expecting clock to be low, and sda to be low */
  printf("sending address: %x\n", address);
  /* Addresses are only 7 bits */
  send_data(h, address << 1, 7);

  return;
}

void send_rw(host *h, enum rw r_w) {
  comp_update_message_state(h->comp, RW);
  switch (r_w) {
  case READ:
    comp_set_sda(h->comp, false);
    break;
  case WRITE:
    comp_set_sda(h->comp, true);
    break;
  default:
    printf("invalid rw state\n");
  }
  comp_set_rw_state(h->comp, r_w);
  update(h);
}

void send_data(host *h, uint8_t data, int len) {

  for (int i = 0; i < len; i++) {
    comp_set_sda(h->comp, data & 0x80);
    data = data << 1;
    comp_toggle_scl(h->comp);
    update(h);
    comp_toggle_scl(h->comp);
  }

  return;
}

void send_message(host *h) {
  printf("sending the start of message\n");
  start_message(h);
  print_state(h);

  /* clock is low, sda is low */
  printf("sending the address\n");
  send_address(h, (*(h->clients + 1))->comp->address);
  print_state(h);

  /* Clock is low */
  printf("sending rw\n");
  comp_toggle_scl(h->comp);
  send_rw(h, READ);
  /* Toggle clock low */
  comp_toggle_scl(h->comp);
  print_state(h);

  /* Read the ack, state: ACK1 */
  printf("Reading the ack\n");
  comp_toggle_scl(h->comp);
  update(h);
  comp_toggle_scl(h->comp);
  print_state(h);

  /* msg = 0x48 atm */

  /* Data1 */
  send_data(h, h->comp->w_buffer[0], 8);
  update_host_clients(h);
  print_state(h);

  /* update message w/ ack */
  update(h);
  print_state(h);

  /* Data2 */
  send_data(h, h->comp->w_buffer[1], 8);
  update_host_clients(h);
  print_state(h);

  /* update message w/ ack */
  update(h);
  print_state(h);

  /* stop */
  end_message(h);
  update(h);
  return;
}

void receive_message(host *h) {}
