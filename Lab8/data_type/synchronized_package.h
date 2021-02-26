#pragma once
#include "data_frame.h"

#define sp_size 3

#define default_synchronized_package                                   \
    {                                                                  \
        {                                                              \
            default_data_frame, default_data_frame, default_data_frame \
        }                                                              \
    }

typedef struct synchronized_package
{
    data_frame packete[sp_size];
} synchronized_package;


void add_data_frame_2_synchronized_package(
    synchronized_package *sp,
    data_frame df)
{

    if (sp->packete[sp_size - 1].msg_id + 1 == df.msg_id)
    {
        for (int i = 0; i < sp_size - 1; i++)
        {
            sp->packete[i] = sp->packete[i + 1];
        }
        sp->packete[sp_size - 1] = df;
    }
}

void print_sp(synchronized_package *sp)
{
    for (int i = 0; i < sp_size; i++)
    {
        print_df(sp->packete + i);
        printf("\n");
    }
}