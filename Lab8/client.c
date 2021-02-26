#include "comunication.h"
#include "./data_type/circular_buffer.h"

int main(int argc, char *argv[])
{

    data_frame data = default_data_frame;

    circular_buffer cb = default_circular_buffer; 

    int32_t client_sock = create_socket(
        PORT_RECIVE_4_SERVER,
        inet_addr("127.0.0.1"),
        connect);



    while (1)
    {
        read_msg(client_sock, (uint8_t *)&data);
        

        add_data_frame_2_circular_buffer(&cb,data);


        print_circular_buffer(&cb);
        printf("\n");
        print_df(&data);
        printf("\n");
    }

    return 0;
}