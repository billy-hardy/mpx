#include "r2.h"
#include "r3.h"


void main() {
  sys_init(MODULE_R3); //don't forget to change version
  r3Init();
  //sys_set_vec(void interrupt (*handler()));//System call handler
  commhand();
  sys_exit();
}
