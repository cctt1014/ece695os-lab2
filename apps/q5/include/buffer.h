#ifndef __BUFFER_H__
#define __BUFFER_H__


#define PROD_N_OBJ_TO_RUN "producer_n.dlx.obj"
#define PROD_O_OBJ_TO_RUN "producer_o.dlx.obj"
#define REAC_1_OBJ_TO_RUN "reaction_1.dlx.obj"
#define REAC_2_OBJ_TO_RUN "reaction_2.dlx.obj"
#define REAC_3_OBJ_TO_RUN "reaction_3.dlx.obj"
#define REAC_4_OBJ_TO_RUN "reaction_4.dlx.obj"

typedef struct atmosphere_stat
{
    int num_n;
    int num_o;
    int num_n2;
    int num_o2;
    int num_no2;
    int num_o3;
} atmosphere_stat;



#endif