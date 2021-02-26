#include "comunication.h"
#include "diverse.h"
#include "crc.h"

int main(int argc, char *argv[])
{

    uint8_t message[MSG_LEN] = "XXAAAABBBBCCCCDDDD";

    // message[0]=0;

    //((uint32_t *)message)

    uint8_t server_reply[MSG_LEN] = {0};

    int32_t send2server = create_socket(
        PORT_SEND_2_SERVER,
        inet_addr("207.180.211.98"),
        connect);

    sleep(1);

    int32_t client_sock = create_socket(
        PORT_RECIVE_4_SERVER,
        inet_addr("207.180.211.98"),
        connect);

    while (1)
    {
        message[0]++;
        message[1] = ~message[1];
        uint32_t rez = rc_crc32(0xFFFFFFFF, message, MSG_LEN - 4);
        *(int *)(message + MSG_LEN - 4) = rez;
        send_msg(send2server, message);
        read_msg(client_sock, message);
        rez = *(int *)(message + MSG_LEN - 4);
        if (rez != rc_crc32(0xFFFFFFFF, message, MSG_LEN - 4))
        {
            printf("Eroare la transmitere, CRC nu e valid !");
        }
        print_as_hex(message);
        sleep(1);
    }

    close(send2server);
    return 0;
}