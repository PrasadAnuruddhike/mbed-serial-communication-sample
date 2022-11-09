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
    INFO("Serial Initialization ..");
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
    if(msg_receive_flag == 1)
    {
        read_data_packet();
    }
}

void read_data_packet()
{
    if(serial_port.readable())
    {
        ThisThread::sleep_for(30ms);
        memset(read_buf, '\0', SERIAL_BUFFER_LENGTH);

        serial_port.read(read_buf, SERIAL_BUFFER_LENGTH);

        char trash[1];
        while(serial_port.readable())
        {
            serial_port.read(&trash, 1);
        }

        printf("\n-------received data-----\n");
        for(int i = 0; i < SERIAL_BUFFER_LENGTH; i++)
        {
            printf("%2X ", read_buf[i]);
        }

        printf("\n-------------------------\n");
        msg_receive_flag = 0;
    }
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


    ThisThread::sleep_for(5s);
    serial_port.write(send_data, SERIAL_BUFFER_LENGTH);
    
}   