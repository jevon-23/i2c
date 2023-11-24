#include <gtest/gtest.h>

extern "C" {
#include "../../include/components.h"
#include "../../include/client.h"
}

TEST(client_tests, new_client) {

  bool **busses = (bool **)malloc(sizeof(bool) * 2);
  *busses = (bool *)malloc(sizeof(bool));
  *(busses + 1) = (bool *)malloc(sizeof(bool));

  bool *sda = *busses;
  bool *scl = *busses + 1;
  *sda = true;
  *scl = true;

  message *msg = new_message();
  enum rw *rw_bus = new_rw();

  component *comp = new_component(0x0, sda, scl, msg, rw_bus);
  client *client = new_client(comp);

  EXPECT_EQ(client->chosen, false);
}
