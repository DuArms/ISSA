#pragma once

#include "../comunication.h"

typedef struct data_frame
{
    uint32_t msg_id;
    int32_t seq_cnt;
    uint8_t payload[500];
    uint32_t crc32;
} data_frame;

#define default_data_frame \
    {                      \
        0, -1, {0}, 0      \
    }

void print_df(data_frame *df)
{   
    printf("{ ");
    printf("ID  =  %d  ", df->msg_id);
    printf("\tSEQ_CNT  =  %d  ", df->seq_cnt);
    printf("\tCRC32  =  %d  ", df->crc32);
    printf(" }");

    return;

    for (int i = 0; i < MSG_LEN - 12; i++)
    {
        printf(" %u", df->payload[i]);
    }
}