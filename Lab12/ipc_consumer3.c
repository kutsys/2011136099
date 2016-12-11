#include "mymsg.h"

int main(int argc, char **argv) {
    int msgid;
    struct my_msg_st some_data;
    long int msg_to_receive = 1;
    const char* my_NAME = "임용민";
    size_t msg_size = MAX_TEXT + sizeof(pid_t);

    // 메시지 대기열 설정
    msgid = msgget((key_t)1256, 0666 | IPC_CREAT);
    if(msgid == -1) {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    printf("Waiting Producer...\n");
    // PRODUCER의 PID와 학번을 메시지 큐로부터 수신
    if(msgrcv(msgid, (void *)&some_data, msg_size, msg_to_receive, 0) == -1) {
        fprintf(stderr, "msgrcv failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    printf("PRODUCER: %d, ID: %s\n", some_data.pid, some_data.some_text);
    
    // PID와 이름을 메세지 큐에 전송 
    some_data.my_msg_type = 2;
    some_data.pid = getpid();
    strcpy(some_data.some_text, my_NAME);

    if(msgsnd(msgid, (void *)&some_data, msg_size, 0) == -1) {
        fprintf(stderr, "msgsnd failed\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
