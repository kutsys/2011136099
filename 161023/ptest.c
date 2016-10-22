#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]) {
    
    /*
     start_time: ptest 최초 구동 시각
     last_time: ptest 현재 구동 시각
     둘의 차이가 ptest 총 구동 시간
     */
    time_t start_time, last_time;
    double diff;
    start_time = time((time_t*)0);

    while(1) {
        last_time = time((time_t*)0);
        printf("ptest run-time %gs\n",difftime(last_time, start_time));
        fflush(stdout);
        sleep(2);
    }
    exit(0);
}
