
#include "tklog.h"

int main(void) {
  printf("Testing \n");

  tklog_configure("*", 5);
  log_info("log_info");

  return 0;
}
