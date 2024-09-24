#ifndef __BUFFER_H__
#define __BUFFER_H__

#define BUFFER_SIZE 64
#define ITEM_SIZE 10
#define BUF_ITEM "0123456789"

#define PROD_OBJ_TO_RUN "producer.dlx.obj"
#define CONS_OBJ_TO_RUN "consumer.dlx.obj"

typedef struct circular_buffer
{
    int head;
    int tail;
    char char_buf[64];
} circular_buffer;



#endif