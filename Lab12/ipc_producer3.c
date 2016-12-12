#include "mymsg.h"

#define INPUT_LENGTH 20

int main(int argc, char **argv) {
    int msgid;
    struct my_msg_st some_data;
    long int msg_to_receive = 2;
    const char* my_ID = "2011136099";
    char str_input[INPUT_LENGTH];
    size_t msg_size = sizeof(some_data) - sizeof(long);

    // 메시지 대기열 설정
    msgid = msgget((key_t)1256, 0666 | IPC_CREAT);
    if(msgid == -1) {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    printf("Enter 'start' to start.\n");
     
        do {
                fgets(str_input, INPUT_LENGTH, stdin);
            } while(strncmp(str_input, "start", 5) != 0);

    // PID와 학번을 메세지 큐에 전송 
    some_data.my_msg_type = 1;
    some_data.pid = getpid();
    strcpy(some_data.some_text, my_ID);

    if(msgsnd(msgid, (void *)&some_data, msg_size, 0) == -1) {
        fprintf(stderr, "msgsnd failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Waiting Consumer...\n");

    // CONSUMER의 PID와 이름을 메시지 큐로부터 수신
    if(msgrcv(msgid, (void *)&some_data, msg_size, msg_to_receive, 0) == -1) {
        fprintf(stderr, "msgrcv failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    printf("PRODUCER: %d, CONSUMER: %d, ID: %s, NAME: %s\n", getpid(), some_data.pid, my_ID, some_data.some_text);

    // 메시지 대기열 삭제
    if(msgctl(msgid, IPC_RMID, 0) == -1) {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
