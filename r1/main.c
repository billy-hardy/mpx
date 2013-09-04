#include "mpx_supt.h"
#include "commhand.h"

void main() {
  sys_init(MODULE_R1); //don't forget to change version
  commhand();
  sys_exit();
}
