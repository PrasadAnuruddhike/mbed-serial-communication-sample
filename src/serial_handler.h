#include "mbed.h"
#include "config.h"

#ifndef SERIAL_HANDLER_H
#define SERIAL_HANDLER_H

#if DEBUG_FLAG == 1
#define DEBUG(x, ...) printf("[DEBUG][SERIAL]" x "\r\n", ##__VA_ARGS__);
#else
#define DEBUG(x, ...)
#endif
#if INFO_FLAG == 1
#define INFO(x, ...) printf("[INFO][SERIAL] " x "\r\n", ##__VA_ARGS__);
#else
#define INFO(x, ...)
#endif
#if PLOT_FLAG == 1
#define PLOT(x, ...) printf("[WARN][SERIAL] " x "\r\n", ##__VA_ARGS__);
#else
#define PLOT(x, ...)
#endif
#if ERR_FLAG == 1
#define ERR(x, ...) printf("[ERROR][SERIAL]" x "\r\n", ##__VA_ARGS__);
#else
#define ERR(x, ...)
#endif


void zhark_serial_init();
void callback_serial();
void msg_check();
void read_data_packet();

void send_message();

#endif // SERIAL_HANDLER_H