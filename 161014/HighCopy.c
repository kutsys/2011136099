#include <unistd.h>     // fstat
#include <sys/stat.h>   // 파일 정보, 시스템 호출들을 사용하는 경우 권장, 일부 UNIX 시스템에서는 필요
#include <stdio.h>  // 표준 I/O 라이브러리
#include <time.h>   // 시간 관련 라이브러리
#include "defbufsize.h"

// default 파일 사이즈
#define DEFAULT_FILE_SIZE 100

// 디버깅을 고려
#ifndef DEBUG
#define DEBUG
#endif

int main(int argc, char* argv[]) {
    char *srcfile, *dstfile;    // 원본파일명, 복사파일명
    char block[USR_BUF];           // buffer, 1K 단위로 복사한다.
    FILE *in, *out;             // 원본파일과 복사파일의 스트림
    int nread;                  // 읽어들인 레코드 개수
    int block_count = 1;        // 1block 복사 카운트
    
    // 파일 사이즈를 추출 및 저장
    struct stat file_info;
    int file_size = 0;
    
    // 실행시 시각 데코레이션을 위한 변수
    struct tm *tm_ptr;
    time_t the_time;
    
    // 파일 복사 부분 실행 시간 측정을 위한 clock_t 변수
    clock_t start_time;
    clock_t end_time;
    
    // 실행시 시각값 설정
    (void) time(&the_time);
    tm_ptr = localtime(&the_time);
    
    // 디버깅시 컴파일 날짜와 시간, line수와 소스 파일명을 출력
#ifdef DEBUG
    printf("Complied: " __DATE__ " at " __TIME__ );
    printf(" (line: %d, file: %s)\n",__LINE__,__FILE__);
#endif
    
    // 실행시 현재 시각 출력
    printf("현재시각: %02d/%02d/%02d ", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday);
    printf("(%02d:%02d:%02d)\n", tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);

    // 시스템 버퍼 사이즈 출력
    printf("System Buffer Size : %d\n",BUFSIZ);
    // 유저 버퍼 사이즈 출력
    printf("usr Buffer size : %d\n",USR_BUF);
    
    // 실행인자로 부터 원본파일과 복사파일의 이름을 입력받음.
    if (argc != 3) {
        printf("사용법: %s [원본 파일명] [복사 파일명]\n", argv[0]);
        return 1;
    }
    
    /* srcfile이 dstfile로 복사된다.
     두번째 실행 인자가 원본 파일명이다.
     세번째 실행 인자가 복사 파일명이다. */
    srcfile = argv[1];
    dstfile = argv[2];
    
    // 원본 파일을 읽기전용 모드로 연다.
    /* "rb" 모드는 파일을 이진파일 읽기 모드로 연다.
     열기 실패시 NULL을 반환한다. */
    // 원본 파일을 열지 못했을 경우에 대한 에러 처리
    if((in = fopen(srcfile, "rb")) == (FILE*)0) {
        perror("원본 파일을 열지 못했습니다.");
        return -1;
    }
    
    // 복사 파일을 쓰기전용 모드로 연다.
    // 복사 파일을 열지 못했을 경우에 대한 에러 처리
    /* "wb" 모드는 파일을 이진파일 쓰기 모드로 연다.
    파일이 없으면 생성하고 있으면 내용을 지운다. */
    if((out = fopen(dstfile, "wb")) == (FILE*)0) {
        perror("복사 파일을 열지 못했습니다.");
        fclose(in);
        return -1;
    }
    
    // 파일 사이즈 추출 및 저장 ('*' 출력 시간 조절)
    if(stat(srcfile, &file_info) == -1)
        file_size = DEFAULT_FILE_SIZE;
    else
        file_size = file_info.st_size / USR_BUF;
    
    // 복사 시작 시간 저장
    start_time = clock();
    
    printf("--------------now copying---------------\n");  
    // 원본 파일로부터 데이터를 읽은 후
    // 읽어들인 데이터의 크기만큼 복사 파일에 작성한다.
    /* fread 함수는 파일의 끝에 도달한 경우에는 읽을 레코드의 갯수
     (sizeof(block))보다 작은 값이 반환될 수 있다. */
    while(((nread = fread(block,sizeof(char), sizeof(block), in)) > 0)) {
        // fwrite함수가 원본 파일에서 읽은 레코드의 갯수 nread보다 적게 기록하면 오류가 발생한 것.
        if(fwrite(block, sizeof(char), nread, out) < nread) {
            perror("파일을 쓰는중 에러가 발생했습니다.");
            break;
        }
        
        block_count++;          // 한 블럭을 복사하면 카운트를 증가시킨다.
        // file_size/10 블럭을 복사하면 하나의 *을 출력한다.
        if (block_count >= file_size/10) {
            block_count = 1;
            printf("*");
            fflush(stdout);
        }
    }
    
    // 복사 끝나는 시각 저장
    end_time = clock();
    printf("\n---------------complete!----------------\n");
    
    // 원본파일과 복사파일을 닫는다.
    fclose(in);
    fclose(out);
    
    // 복사하는데 걸린 시간을 출력한다.
    printf("복사시간: %lfs\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
    
    return 0;
}
