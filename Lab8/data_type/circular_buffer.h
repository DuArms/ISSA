#pragma once

#include "synchronized_package.h"

#define cb_size 3

typedef struct
{
    synchronized_package sp[cb_size];
} circular_buffer;

#define default_circular_buffer                                                                      \
    {                                                                                                \
        {                                                                                            \
            default_synchronized_package, default_synchronized_package, default_synchronized_package \
        }                                                                                            \
    }

void add_data_frame_2_circular_buffer(
    circular_buffer *cb,
    data_frame df)
{

    uint32_t pozitie_inserat = 0;
    synchronized_package *sp_ales = cb->sp + pozitie_inserat;

    for (int i = 0; i < cb_size; i++)
    {
        const synchronized_package *sp = cb->sp + i;
        if (sp->packete[sp_size - 1].seq_cnt == df.seq_cnt)
        {
            pozitie_inserat = i;
            sp_ales = cb->sp + pozitie_inserat;
            break;
        }
        else if (sp->packete[sp_size - 1].seq_cnt < sp_ales->packete[sp_size - 1].seq_cnt)
        {
            pozitie_inserat = i;
            sp_ales = cb->sp + pozitie_inserat;
        }
    }

    if ( sp_ales->packete[sp_size - 1].seq_cnt != df.seq_cnt){
        *sp_ales = (synchronized_package) default_synchronized_package ; 
    }

    //printf("\n %d \n",pozitie_inserat);
    add_data_frame_2_synchronized_package(sp_ales, df);
}

void print_circular_buffer(circular_buffer *cb)
{
    system("clear");
    for (int i = 0; i < cb_size; i++)
    {
        print_sp(cb->sp + i);
        printf("###############################################\n");
    }
}