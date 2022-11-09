#include <mbed.h>
#include "config.h"
#include "serial_handler.h"

#if DEBUG_FLAG == 1
#define DEBUG(x, ...) printf("[DEBUG][MAIN]" x "\r\n", ##__VA_ARGS__);
#else
#define DEBUG(x, ...)
#endif
#if INFO_FLAG == 1
#define INFO(x, ...) printf("[INFO][MAIN] " x "\r\n", ##__VA_ARGS__);
#else
#define INFO(x, ...)
#endif
#if PLOT_FLAG == 1
#define PLOT(x, ...) printf("" x "\r\n", ##__VA_ARGS__);
#else
#define PLOT(x, ...)
#endif
#if ERR_FLAG == 1
#define ERR(x, ...) printf("[ERROR][MAIN] " x "\r\n", ##__VA_ARGS__);
#else
#define ERR(x, ...)
#endif


int main() 
{

  zhark_serial_init();
  
  while(1)
  {
    msg_check();
    send_message();
  }
}