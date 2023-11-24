#include <gtest/gtest.h>

extern "C" {
#include "../../include/components.h"
#include "../../include/client.h"
#include "../../include/host.h"
}

void breakpoint() {
    return;
}

TEST(host_tests, new_host) {

  bool **busses = (bool **)malloc(sizeof(bool) * 2);
  *busses = (bool *)malloc(sizeof(bool));
  *(busses + 1) = (bool *)malloc(sizeof(bool));

  bool *sda = *busses;
  bool *scl = *busses + 1;
  *sda = true;
  *scl = true;

  message *msg = new_message();
  enum rw *rw_bus = new_rw();

  component *host_comp = new_component(0x0, sda, scl, msg, rw_bus);
  component *c1 = new_component(0x7f, sda, scl, msg, rw_bus);
  component *c2 = new_component(0x24, sda, scl, msg, rw_bus);

  client *client1 = new_client(c1);
  client *client2 = new_client(c2);

  client **cc = (client **)malloc(sizeof(client *) * 2);
  *cc = client1;
  *(cc + 1) = client2;

  host *host = new_host(host_comp, cc, 2);

  EXPECT_EQ((*host->clients)->comp->address, 0x7f);
  EXPECT_EQ((*host->clients + 1)->comp->address, 0x24);
  EXPECT_EQ(host->num_clients, 2);
}


TEST(host_tests, start_message) {

  bool **busses = (bool **)malloc(sizeof(bool) * 2);
  *busses = (bool *)malloc(sizeof(bool));
  *(busses + 1) = (bool *)malloc(sizeof(bool));

  bool *sda = *busses;
  bool *scl = *busses + 1;
  *sda = true;
  *scl = true;

  message *msg = new_message();
  enum rw *rw_bus = new_rw();

  component *host_comp = new_component(0x0, sda, scl, msg, rw_bus);
  component *c1 = new_component(0x7f, sda, scl, msg, rw_bus);
  component *c2 = new_component(0x24, sda, scl, msg, rw_bus);

  client *client1 = new_client(c1);
  client *client2 = new_client(c2);

  client **cc = (client **)malloc(sizeof(client *) * 2);
  *cc = client1;
  *(cc + 1) = client2;

  host *host = new_host(host_comp, cc, 2);

  EXPECT_EQ((*host->clients)->comp->address, 0x7f);
  EXPECT_EQ((*host->clients + 1)->comp->address, 0x24);
  EXPECT_EQ(host->num_clients, 2);


  start_message(host);
  EXPECT_EQ(*host->comp->msg->state, EXP_ADDR);
  EXPECT_EQ(*host->comp->sda, false);
  EXPECT_EQ(*host->comp->scl, false);
}


TEST(host_tests, send_address) {

  bool **busses = (bool **)malloc(sizeof(bool) * 2);
  *busses = (bool *)malloc(sizeof(bool));
  *(busses + 1) = (bool *)malloc(sizeof(bool));

  bool *sda = *busses;
  bool *scl = *busses + 1;
  *sda = true;
  *scl = true;

  message *msg = new_message();
  enum rw *rw_bus = new_rw();

  component *host_comp = new_component(0x0, sda, scl, msg, rw_bus);
  component *c1 = new_component(0x7f, sda, scl, msg, rw_bus);
  component *c2 = new_component(0x24, sda, scl, msg, rw_bus);

  client *client1 = new_client(c1);
  client *client2 = new_client(c2);

  client **cc = (client **)malloc(sizeof(client *) * 2);
  *cc = client1;
  *(cc + 1) = client2;

  host *host = new_host(host_comp, cc, 2);

  EXPECT_EQ((*host->clients)->comp->address, 0x7f);
  EXPECT_EQ((*host->clients + 1)->comp->address, 0x24);
  EXPECT_EQ(host->num_clients, 2);
  comp_update_message_state(host->comp, EXP_ADDR);

  breakpoint();
  send_address(host, c2->address);
  EXPECT_EQ(*host->comp->msg->state, RW);

  /* the top bit is shifted off */
  EXPECT_EQ(host->comp->msg->addr, c2->address);
  EXPECT_EQ(*host->comp->msg->msg, c2->address);
  EXPECT_EQ(client2->chosen, true);
  EXPECT_EQ(*host->comp->msg->state, RW);
}

TEST(host_tests, send_rw) {

  bool **busses = (bool **)malloc(sizeof(bool) * 2);
  *busses = (bool *)malloc(sizeof(bool));
  *(busses + 1) = (bool *)malloc(sizeof(bool));

  bool *sda = *busses;
  bool *scl = *busses + 1;
  *sda = true;
  *scl = true;

  message *msg = new_message();
  enum rw *rw_bus = new_rw();

  component *host_comp = new_component(0x0, sda, scl, msg, rw_bus);
  component *c1 = new_component(0x7f, sda, scl, msg, rw_bus);
  component *c2 = new_component(0x24, sda, scl, msg, rw_bus);

  client *client1 = new_client(c1);
  client *client2 = new_client(c2);

  client **cc = (client **)malloc(sizeof(client *) * 2);
  *cc = client1;
  *(cc + 1) = client2;

  host *host = new_host(host_comp, cc, 2);

  EXPECT_EQ((*host->clients)->comp->address, 0x7f);
  EXPECT_EQ((*host->clients + 1)->comp->address, 0x24);
  EXPECT_EQ(host->num_clients, 2);
  comp_update_message_state(host->comp, RW);

  send_rw(host, READ);
  EXPECT_EQ(*host->comp->msg->state, ACK_NAK1);
  EXPECT_EQ(*host->comp->msg->msg, 0x0);

  /* Reset rw, and run again */
  comp_update_message_state(host->comp, RW);
  send_rw(host, WRITE);
  EXPECT_EQ(*host->comp->msg->state, ACK_NAK1);
  EXPECT_EQ(*host->comp->msg->msg, 0x1);
}













