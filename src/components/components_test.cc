#include <gtest/gtest.h>

extern "C" {
#include "../../include/components.h"
}

TEST(message_tests, new_message) {
    message *m = new_message();

    EXPECT_EQ(*m->state, START);
    EXPECT_EQ(m->addr, 0x0);
    EXPECT_EQ(*m->msg, 0x0);
}

TEST(component_tests, new_comp) {
  bool **busses = (bool **)malloc(sizeof(bool) * 2);
  *busses = (bool *)malloc(sizeof(bool));
  *(busses + 1) = (bool *)malloc(sizeof(bool));

  bool *sda = *busses;
  bool *scl = *busses + 1;
  *sda = true;
  *scl = true;

  message *msg = new_message();
  enum rw *rw_bus = new_rw();

  component *c = new_component(0x0, sda, scl, msg, rw_bus);
  EXPECT_EQ(*c->sda, true);
  EXPECT_EQ(*c->scl, true);
  EXPECT_EQ(*c->r_w, READ);
  EXPECT_EQ(c->r_buff_len, 0);
  EXPECT_EQ(c->w_buff_len, 0);
}

TEST(component_tests, comp_toggle_scl) {
  bool **busses = (bool **)malloc(sizeof(bool) * 2);
  *busses = (bool *)malloc(sizeof(bool));
  *(busses + 1) = (bool *)malloc(sizeof(bool));

  bool *sda = *busses;
  bool *scl = *busses + 1;
  *sda = true;
  *scl = true;

  message *msg = new_message();
  enum rw *rw_bus = new_rw();

  component *c = new_component(0x0, sda, scl, msg, rw_bus);
  comp_toggle_scl(c);
  EXPECT_EQ(*c->scl, false);
  comp_toggle_scl(c);
  EXPECT_EQ(*c->scl, true);
}

TEST(component_tests, comp_set_sda) {
  bool **busses = (bool **)malloc(sizeof(bool) * 2);
  *busses = (bool *)malloc(sizeof(bool));
  *(busses + 1) = (bool *)malloc(sizeof(bool));

  bool *sda = *busses;
  bool *scl = *busses + 1;
  *sda = true;
  *scl = true;

  message *msg = new_message();
  enum rw *rw_bus = new_rw();

  component *c = new_component(0x0, sda, scl, msg, rw_bus);
  comp_set_sda(c, false);
  EXPECT_EQ(*c->sda, false);
  comp_set_sda(c, true);
  EXPECT_EQ(*c->sda, true);
}

TEST(component_tests, comp_update_message) {
  bool **busses = (bool **)malloc(sizeof(bool) * 2);
  *busses = (bool *)malloc(sizeof(bool));
  *(busses + 1) = (bool *)malloc(sizeof(bool));

  bool *sda = *busses;
  bool *scl = *busses + 1;
  *sda = true;
  *scl = true;

  message *msg = new_message();
  enum rw *rw_bus = new_rw();

  component *c = new_component(0x0, sda, scl, msg, rw_bus);
  comp_update_message(c, true);
  EXPECT_EQ(*c->msg->msg, 0x1); //0b1
  comp_update_message(c, true);
  EXPECT_EQ(*c->msg->msg, 0x3); //0b11
  comp_update_message(c, false);
  EXPECT_EQ(*c->msg->msg, 0x6);//0b110
  comp_update_message(c, false);
  EXPECT_EQ(*c->msg->msg, 0xc);//0b1100
}

