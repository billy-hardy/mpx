#include "r1.h"

void main() {
  sys_init(MODULE_R2); //don't forget to change version
  queueInit();
  commhand();
  sys_exit();
}
