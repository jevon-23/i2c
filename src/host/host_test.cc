#include <gtest/gtest.h>

extern "C" {
#include "../../include/components.h"
#include "../../include/client.h"
#include "../../include/host.h"
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
  component *c1 = new_component(0xff, sda, scl, msg, rw_bus);
  component *c2 = new_component(0x24, sda, scl, msg, rw_bus);

  client *client1 = new_client(c1);
  client *client2 = new_client(c2);

  client **cc = (client **)malloc(sizeof(client *) * 2);
  *cc = client1;
  *(cc + 1) = client2;

  host *host = new_host(host_comp, cc, 2);

  EXPECT_EQ((*host->clients)->comp->address, 0xff);
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
  component *c1 = new_component(0xff, sda, scl, msg, rw_bus);
  component *c2 = new_component(0x24, sda, scl, msg, rw_bus);

  client *client1 = new_client(c1);
  client *client2 = new_client(c2);

  client **cc = (client **)malloc(sizeof(client *) * 2);
  *cc = client1;
  *(cc + 1) = client2;

  host *host = new_host(host_comp, cc, 2);

  EXPECT_EQ((*host->clients)->comp->address, 0xff);
  EXPECT_EQ((*host->clients + 1)->comp->address, 0x24);
  EXPECT_EQ(host->num_clients, 2);


  start_message(host);
  EXPECT_EQ(*host->comp->msg->state, EXP_ADDR);
  EXPECT_EQ(*host->comp->sda, false);
  EXPECT_EQ(*host->comp->scl, false);

}

















