#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "../sds.h"

#define ITER_NUM 100
#define CHAIN_LEN 100

void randomString(unsigned char message[]){
  int i, r;
  for (i=0;i<8;i++){
    r = rand();
    memcpy(message+i*sizeof(int),&r,sizeof(int));
  }
}

    
int main (void){
    clock_t start, end;

    double cpu_time_keygen=0;
    double cpu_time_sig=0;
    double cpu_time_ver=0;


    SST sst;
    uint256 m[ITER_NUM*CHAIN_LEN];
    VSTclassic vst;
    uint256 sigma[HALF_KEY_NUM];


    srand(time(NULL));
    allocateVSTclassic(&vst,CHAIN_LEN);

    for(size_t i=0;i<ITER_NUM*CHAIN_LEN; i++) randomString(m[i].as_uchar);


    for (size_t i=0; i<ITER_NUM; i++){
        start = end = 0;
        start = clock();
        sds_classic_sot_keygen(&sst,&vst, CHAIN_LEN);
        end = clock();
        cpu_time_keygen += ((double) (end*1000000 - start*1000000));

        for (size_t j=0; j<CHAIN_LEN; j++){
          start = end = 0;
          start = clock();
          sds_classic_sot_sig (sigma, &sst, m[i*ITER_NUM+j]);
          end = clock();
          cpu_time_sig += ((double) (end*1000000 - start*1000000));

          start = end = 0;
          start = clock();
          sds_classic_sot_ver(&vst, m[i*ITER_NUM+j], sigma);
          end = clock();
          cpu_time_ver += ((double) (end*1000000 - start*1000000));
        }
    }

    freeVSTclassic(&vst);


    printf("--------------------SDS for CLASSIC SOT------------------------\n");
    
    printf("Classic SDS keygen time in microseconds per %d calls for %d messages each: %lf\n", ITER_NUM,CHAIN_LEN,cpu_time_keygen/CLOCKS_PER_SEC);
    printf("Classic SDS sig time in microseconds per %d calls: %lf\n", ITER_NUM*CHAIN_LEN,cpu_time_sig/CLOCKS_PER_SEC);
    printf("Classic SDS ver time in microseconds per %d calls: %lf\n", ITER_NUM*CHAIN_LEN,cpu_time_ver/CLOCKS_PER_SEC);
    
    cpu_time_keygen = cpu_time_keygen/ITER_NUM;
    cpu_time_sig = cpu_time_sig/(ITER_NUM*CHAIN_LEN);
    cpu_time_ver = cpu_time_ver/(ITER_NUM*CHAIN_LEN);
    
    printf("Classic SDS keygen time in microseconds per call (for %d messages): %lf\n",CHAIN_LEN, cpu_time_keygen/CLOCKS_PER_SEC);
    printf("Classic SDS sig time in microseconds per call: %lf\n", cpu_time_sig/CLOCKS_PER_SEC);
    printf("Classic SDS ver time in microseconds per call: %lf\n", cpu_time_ver/CLOCKS_PER_SEC);

    printf("--------------------------------------------\n");

    return 0;
}