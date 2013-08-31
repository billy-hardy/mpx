#include "commhand.h"
#include "mpx_supt.h"

int main() {
  sys_init(MODULE_R1); //don't forget to change version
  commhand();
  sys_exit();
}
