#include "r2.h"
#include "r3.h"


void main() {
  sys_init(MODULE_R3); //don't forget to change version
  queueInit();
  r3Init();
  commhand();
  sys_exit();
}
