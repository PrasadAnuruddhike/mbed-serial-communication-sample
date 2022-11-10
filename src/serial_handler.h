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


/*
    HEADER          1-byte          Default: 0xD9
    COMMAND ID      1-byte          
    LENGTH          2-bytes
    PAYLOAD         n-bytes
    CHECKSUM        2-bytes
*/

#define HEADER                                  (0xD6)      // 1 byte
#define COM_WHO_AM_I                            (0x01)
#define COM_ID_ACK                              (0x02)
#define COM_ID_LED_1_RED_CHECK                  (0x03)  

#define MSG_ACK_LENGTH                          8
#define MSG_WAMI_LENGTH                         8

void zhark_serial_init();
void callback_serial();
void msg_check();
int read_data_packet(uint8_t *receive_data);
void decode_packet(uint8_t *receive_data);

void send_msg_acknowledgement(uint8_t com_ID);

void send_message();
void send_who_am_i();

uint16_t generate_checksum(uint8_t *src_data, uint8_t src_data_len);


// move to different cpp
void task_led_check();

#endif // SERIAL_HANDLER_H