#include "mbed.h"
#include "serial_handler.h"

#define MAXIMUM_BUFFER_SIZE         32
#define SERIAL_BUFFER_LENGTH        8

// Switch
static BufferedSerial serial_port(USBTX, USBRX, SERIAL_BAUDRATE);

// KFO
// static BufferedSerial serial_port(P0_25, P0_26, SERIAL_BAUDRATE);

int msg_receive_flag = 0;
uint8_t read_buf[SERIAL_BUFFER_LENGTH];

void zhark_serial_init()
{
    // INFO("Serial Initialization ..");
    serial_port.set_baud(SERIAL_BAUDRATE);
    serial_port.set_format(8, BufferedSerial::None, 1);
    // serial_port.sigio(callback(callback_serial));
    serial_port.sigio(callback_serial);
}

void callback_serial()
{
    msg_receive_flag = 1;
}

void msg_check()
{
    int rxlen = 0;
    int _header = 0;
    int _commandID = 0;

    if(msg_receive_flag == 1)
    {
        rxlen = read_data_packet(read_buf);

        if(rxlen > 0)
        {
            // INFO("RX_Len: %d", rxlen);
            _header         = read_buf[0];
            _commandID      = read_buf[1];

            if(_header == HEADER)
            {
                // INFO("Header Received");
                if(_commandID == COM_ID_LED_1_RED_CHECK)
                {
                    // INFO("Check LED 1 Red");
                    send_msg_acknowledgement(_commandID);
                    task_led_check();
                }
            }
            else
            {
                // INFO("Invalid Command");
            }
        }
    }
}

void send_msg_acknowledgement(uint8_t com_ID)
{
    uint16_t _checksum = 0;
    uint8_t send_data[MSG_ACK_LENGTH];
    send_data[0]    =   HEADER;
    send_data[1]    =   COM_ID_ACK;
    send_data[2]    =   0x02;       // Length
    send_data[3]    =   0x00;       // Length
    send_data[4]    =   com_ID;
    send_data[5]    =   0xFF;       // ACK Received: 0xFF, Task Pass: 0x01; Task Fail: 0x00

    _checksum = generate_checksum(send_data, (MSG_ACK_LENGTH - 2));
    
    send_data[6]    =   _checksum & 0xFF;
    send_data[7]    =   _checksum >> 8;

    serial_port.write(send_data, MSG_ACK_LENGTH);
    ThisThread::sleep_for(200ms);
}

void decode_packet(uint8_t *receive_data)
{

}

int read_data_packet(uint8_t *receive_data)
{
    int _rxlen = 0;
    if(serial_port.readable())
    {
        ThisThread::sleep_for(30ms);
        memset(receive_data, '\0', SERIAL_BUFFER_LENGTH);

        _rxlen = serial_port.read(receive_data, SERIAL_BUFFER_LENGTH);

        char trash[1];
        while(serial_port.readable())
        {
            serial_port.read(&trash, 1);
        }

#ifdef PRINT_SERIAL_RX_BYTES
        printf("\n-------received data-----\n");
        for(int i = 0; i < SERIAL_BUFFER_LENGTH; i++)
        {
            printf("%2X ", read_buf[i]);
        }

        printf("\n-------------------------\n");
#endif  // PRINT_SERIAL_RX_BYTES

        msg_receive_flag = 0;
    }

    return _rxlen;
}

void task_led_check()
{
    uint16_t _checksum = 0;
    bool _task_status = false;
    uint8_t ack_packet[MSG_ACK_LENGTH];

    // INFO("LED Task start");
    // ADD LED Test Task
    ThisThread::sleep_for(5s);
    _task_status = true;

    ack_packet[0]    =   HEADER;
    ack_packet[1]    =   COM_ID_ACK;
    ack_packet[2]    =   0x02;       // Length
    ack_packet[3]    =   0x00;       // Length
    ack_packet[4]    =   COM_ID_LED_1_RED_CHECK;

    if(_task_status)
    {
        ack_packet[5]    =   0x01;       // Task Pass    
    }
    else
    {
        ack_packet[5]    =   0x00;       // Task fail   
    }

    _checksum = generate_checksum(ack_packet, (MSG_ACK_LENGTH - 2));
    
    ack_packet[6]    =   _checksum & 0xFF;
    ack_packet[7]    =   _checksum >> 8;

    serial_port.write(ack_packet, MSG_ACK_LENGTH);
    ThisThread::sleep_for(200ms);
    // INFO("LED Task end");
}

// void msg_check()
// {
//     // int rxlen = 0;
//     // if(msg_receive_flag == 1)
//     // {
//     //     INFO("trig");
//     //     // read_data_packet();

//     //     ThisThread::sleep_for(800ms);

//     //     if(serial_port.readable())
//     //     {
//     //         rxlen = serial_port.read(read_buf, SERIAL_BUFFER_LENGTH);
//     //         INFO("Rxlen : %d", rxlen);
//     //         msg_receive_flag = 0;
//     //     }
//     // }
// }

/*
void read_data_packet()
{   
    // int rxlen = 0;
    // ThisThread::sleep_for(800ms);


    // if(serial_port.readable())
    // {
    //     rxlen = serial_port.read(read_buf, SERIAL_BUFFER_LENGTH);
    //     printf("Rxlen: %d\n", rxlen);
        
    //     msg_receive_flag = 0;
    // }

    // serial_port.sync();
    // printf("t1 \n");
    // if(serial_port.readable())
    // {
    //     printf("t2 \n");
    //     ThisThread::sleep_for(30ms);
    //     memset(read_buf, '\0', SERIAL_BUFFER_LENGTH);

    //     serial_port.read(read_buf, SERIAL_BUFFER_LENGTH);

    //     char trash[1];
    //     while(serial_port.readable())
    //     {
    //         serial_port.read(&trash, 1);
    //     }

    //     printf("\n-------received data-----\n");
    //     for(int i = 0; i < SERIAL_BUFFER_LENGTH; i++)
    //     {
    //         printf("%2X ", read_buf[i]);
    //     }

    //     printf("\n-------------------------\n");
    //     msg_receive_flag = 0;
    // }
}
*/

void send_message()
{
    uint8_t send_data[SERIAL_BUFFER_LENGTH];
    send_data[0]    =   0x48;
    send_data[1]    =   0x45;
    send_data[2]    =   0x4C;
    send_data[3]    =   0x4C;
    send_data[4]    =   0x4F;
    send_data[5]    =   0x00;
    send_data[6]    =   0x0D;
    send_data[7]    =   0x0A;


    // ThisThread::sleep_for(5s);
    wait_us(2000000);
    serial_port.write(send_data, SERIAL_BUFFER_LENGTH);
    
}

void send_who_am_i()
{
    uint8_t send_data[MSG_WAMI_LENGTH];
    uint16_t _checksum = 0;

    send_data[0]    =   HEADER;
    send_data[1]    =   COM_WHO_AM_I;
    send_data[2]    =   0x02;
    send_data[3]    =   0x00;
    send_data[4]    =   HEADER;
    send_data[5]    =   HEADER;

    _checksum = generate_checksum(send_data, (MSG_WAMI_LENGTH - 2));
    
    send_data[6]    =   _checksum & 0xFF;
    send_data[7]    =   _checksum >> 8;

    serial_port.write(send_data, SERIAL_BUFFER_LENGTH);
    ThisThread::sleep_for(500ms);
}

uint16_t generate_checksum(uint8_t *src_data, uint8_t src_data_len)
{
    /*
        CRC-16/CCITT-FALSE format 
    */
    uint16_t crc = 0xFFFF;
    uint16_t checksum;  

    // Calculate Checksum  
    for(int i = 0; i < src_data_len; i++){
        crc= ((uint8_t)(crc >> 8) | (crc << 8))^src_data[i];
        crc ^= (uint8_t)(crc & 0xFF) >>4;
        crc ^= (crc << 12);
        crc ^= ((crc & 0xFF) <<5);
    }

    checksum = crc;
    return checksum;
}