#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <inttypes.h>
#include <stdbool.h>
/*
 * I2C components have:
 *  - sda -> seerial data
 *    host & client send data
 *  - scl -> serial clock
 *    line for clock signal
 *  - address
 *  - 5 message buffer for send & receive
 *    (data portion of each message is 2 8 bit segments)
 *
 */

enum msg_state {
    START,
    EXP_ADDR,
    RW,
    ACK_NAK1,
    DATA1,
    ACK_NACK2,
    DATA2,
    ACK_NACK3,
    STOP
};

/* Message structure */
typedef struct message {
    enum msg_state *state;
    uint8_t addr;
    uint32_t *msg;
} message;

message *new_message();
void reset_message_bus(message *msg);

/* RW Enum */
enum rw {
    READ,
    WRITE,
};

enum rw *new_rw();


/* Component structure */
typedef struct component {
  bool *sda;
  bool *scl;
  uint8_t address; /* 7 bit address */
  message *msg;
  enum rw *r_w;
  uint8_t r_buffer[10];
  uint8_t r_buff_len;
  uint8_t w_buffer[10];
  uint8_t w_buff_len;
} component;

component *new_component(uint8_t address, bool *sda, bool *scl, message *msg_bus, enum rw *rw_bus);

void comp_toggle_scl(component *comp);
void comp_set_sda(component *comp, bool bit);
void comp_update_message(component *comp, bool bit);
void comp_update_message_state(component *comp, enum msg_state state);

void comp_set_rw_state(component *comp, enum rw r_w);

#endif
