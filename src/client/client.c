#include "../../include/client.h"
#include <stdio.h>
#include <stdlib.h>

/* A client cannot drive sda high, can only pull it low */

client *new_client(component *comp) {
  client *c = (client *)malloc(sizeof(client));
  c->comp = comp;
  c->chosen = false;
  return c;
}

void print_client(client *c) {
  printf("client {addr : %x; chosen : %d\n", c->comp->address, c->chosen);
  printf("read buff: %x\n", c->comp->r_buffer[c->comp->r_buff_len - 1]);
}

void update_client(client *c) {
  /* If we are not the chosen client, or we are not currently checking to see
   * for destination address (EXP_ADRR) */

  if (!c->chosen && *c->comp->msg->state != EXP_ADDR)
    return;

  switch (*c->comp->msg->state) {
  case EXP_ADDR:
    if (c->comp->address == c->comp->msg->addr)
      c->chosen = true;
    break;
  case ACK_NAK1:
    /* SCL should be low when entering here */
    /* Send ack bit by setting sda low, updating state */
    comp_set_sda(c->comp, false);
    break;
  case DATA1:
    if (*c->comp->r_w == READ) {
      /* read the last byte of the message, store in read buff */
      c->comp->r_buffer[c->comp->r_buff_len] = *c->comp->msg->msg & 0xff;
      c->comp->r_buff_len++;
    }
    /* Send ack bit by setting sda low, updating state */
    printf("client read data, sda set to low\n");
    comp_set_sda(c->comp, false);
    comp_update_message_state(c->comp, ACK_NACK2);
    break;
  case DATA2:
    if (*c->comp->r_w == READ) {
      /* read the last byte of the message, store in read buff */
      c->comp->r_buffer[c->comp->r_buff_len] = *c->comp->msg->msg & 0xff;
      c->comp->r_buff_len++;
    }
    /* Send ack bit by setting sda low, updating state */
    printf("client read data, sda set to low\n");
    comp_set_sda(c->comp, false);
    comp_update_message_state(c->comp, ACK_NACK3);
    c->chosen = false;
    break;
  case RW:
  case START:
  case ACK_NACK2:
  case ACK_NACK3:
    break;
  default:
    printf("invalid state\n");
    exit(-1);
  }
}
