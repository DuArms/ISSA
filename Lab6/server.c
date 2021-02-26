#include "comunication.h"
#include "diverse.h"
#include <pthread.h>
#include "crc.h"

struct connection_handler_arguments
{
    uint32_t client_socket;
    struct in_addr adresa;
};

void *connection_handler(void *argv)
{
    struct connection_handler_arguments arguments = *(struct connection_handler_arguments *)argv;

    uint32_t client_socket = arguments.client_socket;

    uint8_t message[MSG_LEN] = {0};

    // char *str = calloc(INET_ADDRSTRLEN, sizeof(char));
    // inet_ntop(AF_INET, &arguments.adresa, str, INET_ADDRSTRLEN);

    // int32_t locatie_client = create_socket(PORT_RECIVE_4_SERVER, inet_addr("127.0.0.1"), connect);

    int32_t receve4server = create_socket(
        PORT_RECIVE_4_SERVER,
        INADDR_ANY,
        bind);

    struct sockaddr_in server;
    listen(receve4server, 1);
    uint32_t client_sock = accept_socket(receve4server, server);

    // free(str);

    while (true)
    {
        read_msg(client_socket, message);
        print_as_hex(message);
        printf("%s", message);
        uint32_t rez = *(int *)(message + MSG_LEN - 4);
        if (rez != rc_crc32(0xFFFFFFFF, message, MSG_LEN - 4))
        {
            printf("Eroare la transmitere, CRC nu e valid !");
        }
        message[0]++;
        message[1] = ~message[1];
        rez = rc_crc32(0xFFFFFFFF, message, MSG_LEN - 4);
        *(int *)(message + MSG_LEN - 4) = rez;
        send_msg(client_sock, message);
        sleep(1);
    }
    free(argv);
}

int main(int argc, char *argv[])
{
    struct sockaddr_in client;

    uint32_t receptionare_date = create_socket(PORT_SEND_2_SERVER,
                                               INADDR_ANY,
                                               bind);

    printf("Waiting for incoming connections...");

    listen(receptionare_date, 3);

    while (true)
    {
        uint32_t client_sock = accept_socket(receptionare_date, client);
        puts("Connection accepted");

        pthread_t new_thread;

        struct connection_handler_arguments *arguments =
            malloc(sizeof(struct connection_handler_arguments));

        arguments->client_socket = client_sock;
        arguments->adresa = client.sin_addr;

        if (pthread_create(&new_thread, NULL, connection_handler, (void *)arguments) < 0)
        {
            perror("could not create thread");
            return 1;
        }

        pthread_join(new_thread, NULL);
    }

    return 0;
}